//*******************************ControlTask***********************************
//Copyright (c) 2025 Trenser Technology Solutions
//All Rights Reserved
//*****************************************************************************
//
//File     : ControlTask.c
//Summary  : Infinitely check the button press and send the request to Display
//			 Task and Request to Change the DISPLAY Mode
//Note     : None
//Author   : Drisya P
//Date     : 25/Jul/2025
//
//*****************************************************************************

//*********************Include Files*******************************************
#include <stdio.h>
#include <stdbool.h>
#include "ControlTask.h"
#include "gpio.h"
#include "AppMain.h"
#include "osMsgq.h"
#include "osTask.h"

//*********************Local Types*********************************************

//*********************Local Constants*****************************************

//*********************Local Variables*****************************************
static uint16 sgunUIdCounter = 0;
static int16 punPayLoad[3] = {0x0000, 0x0000, 0x0001};
//*********************Local Functions*****************************************
static bool ControlTaskBuildRequest(REQUEST_MSG*);

//*********************.ControlTask.********************************************
//Purpose :	Infinitely check the button press and send the request to Display
//			Task to Change the Mode
//Inputs  : None
//Outputs : None
//Return  : None
//Notes   : None
//*****************************************************************************
void ControlTask()
{
	REQUEST_MSG stReqMsg = {0, 0, 0, 0, 0};
	ACK_MSG stAckMsg = {0, 0, 0};

	if(osMsgqDisplayToControlInit(sizeof(stAckMsg)))
	{
		while(1)
		{
			if(gpioReadButtonPress())
			{
				LOG("\nButton Pressed\r\n");

				if(ControlTaskBuildRequest(&stReqMsg))
				{
					LOG("Control Task:Request Building\r\n");

					if(osMsgqMessageSendToDisplay(stReqMsg))
					{
						osTaskDelay(300);
					}

					if(osMsgqMessageRcvInControl(&stAckMsg))
					{
						LOG("Control Task: ACKUID=%lu, CMD=0x%02X, "
									"STATE=0x%02X\r\n\n", \
								stAckMsg.ulUId, stAckMsg.ucCmd,
								stAckMsg.ucState);
					}
				}
				osTaskDelay(DELAY_100);
			}
		}
	}
}

//*********************.ControlTaskBuildRequest.*******************************
//Purpose :	Build the Request Message
//Inputs  : stReqMsg - Request Message
//Outputs : None
//Return  : TRUE - Request Message built, FALSE - error
//Notes   : None
//*****************************************************************************
static bool ControlTaskBuildRequest(REQUEST_MSG* stReqMsg)
{
	bool blFlag = FALSE;

	if(stReqMsg != NULL)
	{
		stReqMsg->unUId = ++sgunUIdCounter;
		stReqMsg->ucCmd = CHANGE_MODE;
		stReqMsg->unLength = CONTROL_PAYLOAD_LENGTH;
		stReqMsg->punPayLoad = punPayLoad;
		stReqMsg->ucCheckSum = CHECKSUM;
		blFlag = TRUE;
	}

	return blFlag;
}

//EOF
