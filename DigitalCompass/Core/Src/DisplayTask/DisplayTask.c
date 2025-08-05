//*******************************DisplayTask**********************************
//Copyright (c) 2025 Trenser Technology Solutions
//All Rights Reserved
//*****************************************************************************
//
//File     : DisplayTask.c
//Summary  : Receive the Request from Control Task to change Display Mode and
//			 Receives the AcceleroMeter Sensor Data From Sensor Task.
//Note     : None
//Author   : Drisya P
//Date     : 25/Jul/2025
//
//*****************************************************************************

//*********************Include Files*******************************************
#include <stdio.h>
#include <stdbool.h>
#include "DisplayTask.h"
#include "AppMain.h"
#include "osMsgq.h"
#include "osTask.h"
#include "i2c_hal.h"

//*********************Local Types*********************************************

//*********************Local Constants*****************************************

//*********************Local Variables*****************************************
static uint32 sgulUId = 0;
static uint16 sgunLength = 0;
volatile DISPLAYMODE eCurrentDisplayMode = DISPLAY_MODE_DATA;
static COORDINATES gstCoordinate = {0, 0, 0};
static uint16 gunAngle = 0;

//*********************Local Functions*****************************************
static bool DisplayTaskSetAckMsg(ACK_MSG*, uint8);
static bool DisplayTaskProcessRequest(REQUEST_MSG*);
static bool DisplayTaskProcessUID(REQUEST_MSG*);
static bool DisplayTaskProcessCMD(REQUEST_MSG*);
static bool DisplayTaskProcessPayLoad(REQUEST_MSG*);
static bool DisplayTaskProcessLength(REQUEST_MSG*);
static bool DisplayTaskProcessChecksum(REQUEST_MSG*);
static bool DisplayTaskHandleRequests(REQUEST_MSG*);
static bool DisplayTaskHandleDisplay();
static bool DisplayTaskModeSwitch();
static bool DisplayTaskNeedlehange();

//*********************.DisplayTask.******************************************
//Purpose : Receive the Request from Control Task to change Display Mode and
//			 Receives the AcceleroMeter Sensor Data From Sensor Task.
//Inputs  : None
//Outputs : None
//Return  : None
//Notes   : None
//*****************************************************************************
void DisplayTask()
{
	REQUEST_MSG stReqMsg = {0, 0, 0, 0, 0};

	if(osMsgqToDisplayInit(sizeof(stReqMsg)))
	{
		if(!DisplayTaskHandleDisplay())
		{
			LOG("Display Handling Failed\r\n");
		}

		while(1)
		{
			if(DisplayTaskHandleRequests(&stReqMsg))
			{
				LOG("Display Task - Completed Request\r\n\n");
			}

			osTaskDelay(DELAY_50);
		}
	}
}

//*********************.DisplayTaskHandleRequests.*****************************
//Purpose : Handle the communication with  ControlTask and Sensor Task
//Inputs  : stReqMsg - Request  Message from Control Task
//Outputs : None
//Return  : TRUE - SUCCESS , FALSE -  FAILURE
//Notes   : None
//*****************************************************************************
static bool DisplayTaskHandleRequests(REQUEST_MSG* stReqMsg)
{
	bool blFlag = FALSE;
	uint8 ucStatus = 0;
	ACK_MSG stAckMsg = {0, 0, 0};

	if(osMsgqMessageRcvInDisplay(stReqMsg))
	{
		LOG("Display Task: REQUID=%du CMD=0x%02X LENGTH=%du \
					PAYLOAD=0x%04X 0x%04X 0x%04X CHECKSUM=0x%02X\r\n", \
				stReqMsg->unUId, stReqMsg->ucCmd, stReqMsg->unLength, \
				stReqMsg->punPayLoad[0], stReqMsg->punPayLoad[1],
				stReqMsg->punPayLoad[2], stReqMsg->ucCheckSum);
		blFlag = DisplayTaskProcessRequest(stReqMsg);
		ucStatus = (blFlag != FALSE) ? ACK_STATUS_OK : ACK_STAUS_ERROR;

		if(DisplayTaskSetAckMsg(&stAckMsg, ucStatus))
		{
			if(stReqMsg->ucCmd == CMD_CHANGE_MODE)
			{
				if(osMsgqMessageSendToControl(stAckMsg))
				{
					blFlag = TRUE;
				}
			}
			else
			{
				if(osMsgqMessageSendToSensor(stAckMsg))
				{
					blFlag = TRUE;
				}
			}

			osTaskDelay(DELAY_200);
		}
	}

	return blFlag;
}

//*********************.DisplayTaskHandleDisplay.******************************
//Purpose : Handle the communication with  ControlTask
//Inputs  : None
//Outputs : None
//Return  : TRUE - SUCCESS , FALSE -  FAILURE
//Notes   : None
//*****************************************************************************
static bool DisplayTaskHandleDisplay()
{
	bool blFlag = FALSE;

	do
	{
		if(!i2cHalDisplayInit())
		{
			break;
		}

		blFlag = TRUE;
	}while(0);

	return blFlag;
}

//*********************.DisplayTaskSetAckMsg.**********************************
//Purpose : Set Ack Message according to Cmd and Request Processing.
//Inputs  : stReqMsg - Request Msg from Control or Sensor Task,
//			ucState - Ack State Ok or Error
//Inputs  : stAckMsg - Ack Message that going to send from Display Task
//Outputs : None
//Return  : TRUE - SUCCESS , FALSE -  FAILURE
//Notes   : None
//*****************************************************************************
static bool DisplayTaskSetAckMsg(ACK_MSG* stAckMsg, uint8 ucState)
{
	bool blFlag = FALSE;
	CMD_TYPE cmd = CMD_ACK;

	if(stAckMsg != NULL)
	{
		stAckMsg->ulUId = sgulUId;
		stAckMsg->ucCmd = cmd;
		stAckMsg->ucState = ucState;
		blFlag = TRUE;
	}

	return blFlag;
}

//*********************.DisplayTaskProcessRequest.*****************************
//Purpose : Process Request Message received from Control Task or Sensor Task
//Inputs  : stReqMsg - Request Msg from Control Task or Sensor Task
//Outputs : None
//Return  : TRUE - Request Message Processed, FALSE - error
//Notes   : None
//*****************************************************************************
static bool DisplayTaskProcessRequest(REQUEST_MSG* stReqMsg)
{
	bool blFlag = FALSE;

	do
	{
		if(stReqMsg == NULL)
		{
			break;
		}

		if(!DisplayTaskProcessUID(stReqMsg))
		{
			break;
		}

		if(!DisplayTaskProcessChecksum(stReqMsg))
		{
			break;
		}

		if(!DisplayTaskProcessLength(stReqMsg))
		{
			break;
		}

		if(!DisplayTaskProcessPayLoad(stReqMsg))
		{
			break;
		}

		if(!DisplayTaskProcessCMD(stReqMsg))
		{
			break;
		}

		// If all succeeded
		blFlag = TRUE;

	}while(0);

	return blFlag;
}

//*********************.DisplayTaskProcessUID.********************************
//Purpose : Process UID of Request Message
//Inputs  : stReqMsg - Request Msg from Control Task or Sensor Task
//Outputs : None
//Return  : TRUE - Request Message UID processed, FALSE - error
//Notes   : None
//*****************************************************************************
static bool DisplayTaskProcessUID(REQUEST_MSG* stReqMsg)
{
	bool blFlag = FALSE;

	if(stReqMsg != NULL)
	{
		sgulUId =  (uint16)stReqMsg->unUId;
		blFlag = TRUE;
	}

	return blFlag;
}

//*********************.DisplayTaskProcessCMD.*********************************
//Purpose : Process CMD of the Request Message
//Inputs  : stReqMsg - Request Msg from Control Task or Sensor Task
//Outputs : None
//Return  : TRUE - Request Message CMD processed, FALSE - error
//Notes   : None
//*****************************************************************************
static bool DisplayTaskProcessCMD(REQUEST_MSG* stReqMsg)
{
	bool blFlag = FALSE;

	if(stReqMsg != NULL)
	{
		switch(stReqMsg->ucCmd)
		{
			case CMD_ACCELEROMETER_DATA:
				if(i2cHalDisplayCoordinates(gstCoordinate))
				{
					blFlag = TRUE;
				}

				break;

			case CMD_COMPASS_DATA:
				if(DisplayTaskNeedlehange())
				{
					blFlag = TRUE;
				}

				break;

			case CMD_CHANGE_MODE:
				if(DisplayTaskModeSwitch())
				{
					blFlag = TRUE;
				}

				break;

			default:
				printf("Invalid Command: 0x%02X\r\n", stReqMsg->ucCmd);
				break;
		}
	}

	return blFlag;
}

//*********************.DisplayTaskProcessLength.******************************
//Purpose : Process Length of Request Message
//Inputs  : stReqMsg - Request Msg from Control Task or Sensor Task
//Outputs : None
//Return  : TRUE - Request Message Length processed, FALSE - error
//Notes   : None
//*****************************************************************************
static bool DisplayTaskProcessLength(REQUEST_MSG* stReqMsg)
{
	bool blFlag = FALSE;

	if(stReqMsg != NULL)
	{
		sgunLength = stReqMsg->unLength;
		blFlag = TRUE;
	}

	return blFlag;
}

//*********************.DisplayTaskProcessPayLoad.*****************************
//Purpose : Process PayLoad of Request Message
//Inputs  : stReqMsg - Request Msg from Control Task or Sensor Task
//Outputs : None
//Return  : TRUE - Request Message PayLoad processed, FALSE - error
//Notes   : None
//*****************************************************************************
static bool DisplayTaskProcessPayLoad(REQUEST_MSG* stReqMsg)
{
	bool blFlag = FALSE;
	uint8 ucPayLoadLength = 0;

	if(stReqMsg != NULL)
	{
		ucPayLoadLength = sizeof(stReqMsg->punPayLoad[0]) +
										  sizeof(stReqMsg->punPayLoad[1]) +
										  sizeof(stReqMsg->punPayLoad[2]);

		if(sgunLength == ucPayLoadLength)
		{
			if(stReqMsg->ucCmd == CMD_ACCELEROMETER_DATA)
			{
				gstCoordinate.nX = stReqMsg->punPayLoad[0];
				gstCoordinate.nY = stReqMsg->punPayLoad[1];
				gstCoordinate.nZ = stReqMsg->punPayLoad[2];
			}

			if(stReqMsg->ucCmd == CMD_COMPASS_DATA)
			{
				gunAngle = stReqMsg->punPayLoad[0];
			}

			blFlag = TRUE;
		}
	}

	return blFlag;
}

//*********************.DisplayTaskProcessChecksum.****************************
//Purpose : Process Checksum of Request Message
//Inputs  : stReqMsg - Request Msg from Control Task or Sensor Task
//Outputs : None
//Return  : TRUE - Request Message Checksum processed, FALSE - error
//Notes   : None
//*****************************************************************************
static bool DisplayTaskProcessChecksum(REQUEST_MSG* stReqMsg)
{
	bool blFlag = FALSE;
	uint8 ucReqMsgSize = 0;

	if(stReqMsg != NULL)
	{
		ucReqMsgSize = sizeof(stReqMsg->unUId) + sizeof(stReqMsg->ucCmd) +
			sizeof(stReqMsg->unLength) + sizeof(stReqMsg->punPayLoad[0]) +
			sizeof(stReqMsg->punPayLoad[1]) + sizeof(stReqMsg->punPayLoad[2]) +
			sizeof(stReqMsg->ucCheckSum) ;

		if(ucReqMsgSize - 1 == stReqMsg->ucCheckSum)
		{
			blFlag = TRUE;
		}
	}

	return blFlag;
}

//*********************.DisplayTaskModeSwitch.*********************************
//Purpose : Process modeSwitch task of the Request Message
//Inputs  : stReqMsg - Request Msg from Control Task or Sensor Task
//Outputs : None
//Return  : TRUE - Request Message Mode Change CMD processed, FALSE - error
//Notes   : None
//*****************************************************************************
static bool DisplayTaskModeSwitch()
{
	bool blFlag =  FALSE;

	eCurrentDisplayMode = (eCurrentDisplayMode == DISPLAY_MODE_COMPASS) ?
			DISPLAY_MODE_DATA : DISPLAY_MODE_COMPASS;

	if(i2cHalChangeDisplayMode(eCurrentDisplayMode))
	{
		LOG("I2C DISPLAY - Display Mode Changed --->");
		if (eCurrentDisplayMode == DISPLAY_MODE_DATA)
		{
			LOG("DATA MODE\r\n");
		}
		else
		{
			LOG("COMPASS MODE\r\n");
		}
		blFlag = TRUE;
	}

	return blFlag;
}

//*********************.DisplayTaskNeedlehange.********************************
//Purpose : Process Change the Needle Position of the Request Message
//Inputs  : stReqMsg - Request Msg from Control Task or Sensor Task
//Outputs : None
//Return  : TRUE - Request Message CMD processed, FALSE - error
//Notes   : None
//*****************************************************************************
static bool DisplayTaskNeedlehange()
{
	bool blFlag = FALSE;

	if(i2cHalChangeNeedlePosition(gunAngle))
	{
		blFlag = TRUE;
	}

	return blFlag;
}
//EOF
