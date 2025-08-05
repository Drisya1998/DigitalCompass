//**************************** osMsgq *****************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// Summary : osMsgq header File
// Note    : Added Macros
//
//*****************************************************************************

#ifndef INC_OS_MSGQ_H_
#define INC_OS_MSGQ_H_

//******************************* Include Files *******************************
#include "cmsis_os2.h"
#include "stdbool.h"
#include "osTask.h"

//******************************* Global Types ********************************

//***************************** Global Constants ******************************
#define MSG_COUNT			10
#define MSG_PRIORITY		0

//***************************** Global Variables ******************************
extern osMessageQueueId_t ControlToDisplayId;
extern osMessageQueueId_t DisplayToControlId;
extern osMessageQueueId_t SensorToDisplayd;
extern osMessageQueueId_t DisplayToSensorId;

//***************************** Function Declaration **************************
//Message queue in Display Task
bool osMsgqToDisplayInit(uint32);
bool osMsgqMessageSendToDisplay(REQUEST_MSG);
bool osMsgqMessageRcvInDisplay(REQUEST_MSG*);

//Control and Display Message Queue
bool osMsgqDisplayToControlInit(uint32);
bool osMsgqMessageSendToControl(ACK_MSG);
bool osMsgqMessageRcvInControl(ACK_MSG*);

//Sensor and Display Message Queue
bool osMsgqDisplayToSensorInit(uint32);
bool osMsgqMessageSendToSensor(ACK_MSG);
bool osMsgqMessageRcvInSensor(ACK_MSG*);


//*********************** Inline Method Implementations ***********************

#endif /* INC_OS_MSGQ_H_ */
