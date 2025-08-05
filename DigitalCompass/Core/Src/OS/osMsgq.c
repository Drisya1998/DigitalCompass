//*******************************osMsgq****************************************
//Copyright (c) 2025 Trenser Technology Solutions
//All Rights Reserved
//*****************************************************************************
//
//File     : osMsgq.c
//Summary  : Handles message queue operaions
//Note     : None
//Author   : Drisya P
//Date     : 25/Jul/2025
//
//*****************************************************************************

//*********************Include Files*******************************************
#include <osMsgq.h>
#include <stdio.h>
#include <stdbool.h>
#include "AppMain.h"
#include "cmsis_os2.h"

//*********************Local Types*********************************************

//*********************Local Constants*****************************************

//*********************Local Variables*****************************************
osMessageQueueId_t MsgqToDisplayId = NULL;
osMessageQueueId_t DisplayToControlId = NULL;
osMessageQueueId_t DisplayToSensorId = NULL;
//*********************Local Functions*****************************************


//*********************.osMsgqToDisplayInit.***********************************
//Purpose :	initialize Message Queue from Control Task and sensor task
//			to Display Task
//Inputs  : MsgSize - Size of Request Message
//Outputs : None
//Return  : TRUE - Message Queue Initialization completed, FALSE - error
//Notes   : None
//*****************************************************************************
bool osMsgqToDisplayInit(uint32 MsgSize)
{
	bool blFlag = FALSE;

	MsgqToDisplayId = osMessageQueueNew(MSG_COUNT, MsgSize, NULL);

	if(MsgqToDisplayId != NULL)
	{
		blFlag = TRUE;
	}

	return blFlag;
}

//*********************.osMsgqDisplayToControlInit.****************************
//Purpose :	initialize Message Queue from Display Task to Control Task
//Inputs  : MsgSize - Size of Ack Message
//Outputs : None
//Return  : TRUE - Message Queue Initialization completed, FALSE - error
//Notes   : None
//*****************************************************************************
bool osMsgqDisplayToControlInit(uint32 MsgSize)
{
	bool blFlag = FALSE;

	DisplayToControlId = osMessageQueueNew(MSG_COUNT, MsgSize, NULL);

	if(DisplayToControlId != NULL)
	{
		blFlag = TRUE;
	}

	return blFlag;
}

//*********************.osMsgqMessageSendToDisplay.****************************
//Purpose :	Send Message to Display through Message Queue
//Inputs  : stReqMsg -  Request Message
//Outputs : None
//Return  : TRUE - Message sent, FALSE - error
//Notes   : None
//*****************************************************************************
bool osMsgqMessageSendToDisplay(REQUEST_MSG stReqMsg)
{
	bool blFlag = FALSE;

	if(osMessageQueuePut(MsgqToDisplayId, &stReqMsg, MSG_PRIORITY,
			0) == osOK)
	{
		blFlag = TRUE;
	}

	return blFlag;
}

//*********************.osMsgqMessageRcvInDisplay.*****************************
//Purpose :	Receive Message from Control through Message Queue
//Inputs  : stReqMsg -  Request Message
//Outputs : None
//Return  : TRUE - Message Received, FALSE - error
//Notes   : None
//*****************************************************************************
bool osMsgqMessageRcvInDisplay(REQUEST_MSG* stReqMsg)
{
	bool blFlag = FALSE;

	if(stReqMsg != NULL)
	{
		if (osMessageQueueGet(MsgqToDisplayId, stReqMsg, NULL,
				100) == osOK)
		{
			blFlag = TRUE;
		}
	}

	return blFlag;
}

//*********************.osMsgqMessageSendToControl.****************************
//Purpose :	Send Message to Control through Message Queue
//Inputs  : stAckMsg -  Ack Message
//Outputs : None
//Return  : TRUE - Message Sent, FALSE - error
//Notes   : None
//*****************************************************************************
bool osMsgqMessageSendToControl(ACK_MSG stAckMsg)
{
	bool blFlag = FALSE;

	if(osMessageQueuePut(DisplayToControlId, &stAckMsg, MSG_PRIORITY,
			                                 0) == osOK)
	{
		blFlag = TRUE;
	}

	return blFlag;
}

//*********************.osMsgqMessageRcvInControl.*****************************
//Purpose :	Receive Message from Display through Message Queue
//Inputs  : stAckMsg -  Ack Message
//Outputs : None
//Return  : TRUE - Message Received, FALSE - error
//Notes   : None
//*****************************************************************************
bool osMsgqMessageRcvInControl(ACK_MSG* stAckMsg)
{
	bool blFlag = FALSE;

	if(stAckMsg != NULL)
	{
		if (osMessageQueueGet(DisplayToControlId, stAckMsg, NULL,
				osWaitForever) == osOK)
		{
			blFlag = TRUE;
		}
	}

	return blFlag;
}

//*********************.osMsgqDisplayToSensorInit.****************************
//Purpose :	initialize Message Queue from Display Task to Sensor Task
//Inputs  : MsgSize - Size of Ack Message
//Outputs : None
//Return  : TRUE - Message Queue Initialization completed, FALSE - error
//Notes   : None
//*****************************************************************************
bool osMsgqDisplayToSensorInit(uint32 MsgSize)
{
	bool blFlag = FALSE;

	DisplayToSensorId = osMessageQueueNew(MSG_COUNT, MsgSize, NULL);

	if(DisplayToSensorId != NULL)
	{
		blFlag = TRUE;
	}

	return blFlag;
}

//*********************.osMsgqMessageSendToSensor.*****************************
//Purpose :	Send Message to Receiver through Message Queue
//Inputs  : stAckMsg -  Ack Message
//Outputs : None
//Return  : TRUE - Message Sent, FALSE - error
//Notes   : None
//*****************************************************************************
bool osMsgqMessageSendToSensor(ACK_MSG stAckMsg)
{
	bool blFlag = FALSE;

	if(osMessageQueuePut(DisplayToSensorId, &stAckMsg, MSG_PRIORITY,
												 0) == osOK)
	{
		blFlag = TRUE;
	}

	return blFlag;
}

//*********************.osMsgqMessageRcvInSensor.******************************
//Purpose :	Receive Message from Display through Message Queue
//Inputs  : stAckMsg -  Ack Message
//Outputs : None
//Return  : TRUE - Message Received, FALSE - error
//Notes   : None
//*****************************************************************************
bool osMsgqMessageRcvInSensor(ACK_MSG* stAckMsg)
{
	bool blFlag = FALSE;

	if(stAckMsg != NULL)
	{
		if (osMessageQueueGet(DisplayToSensorId, stAckMsg, NULL,
				osWaitForever) == osOK)
		{
			blFlag = TRUE;
		}
	}

	return blFlag;
}

//EOF
