//**************************** SensorTask *************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// Summary : SensorTask header File
// Note    : Added Macros
//
//*****************************************************************************

#ifndef INC_SENSOR_TASK_H_
#define INC_SENSOR_TASK_H_

//******************************* Include Files *******************************

//******************************* Global Types ********************************

//***************************** Global Constants ******************************

//***************************** Global Variables ******************************
#define ACCELEROMETER_DATA			0x01
#define COMPASS_DATA				0x02
#define PAYLOAD_LENGTH				6
#define SENSOR_CHECKSUM				11
//***************************** Function Declaration **************************
void SensorTask();

//*********************** Inline Method Implementations ***********************

#endif /* INC_SENSOR_TASK_H_ */
