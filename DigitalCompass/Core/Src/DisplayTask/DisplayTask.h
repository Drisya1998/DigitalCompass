//**************************** DisplayTask ***********************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// Summary : DisplayTask header File
// Note    : Added enum for CMD Type
//
//*****************************************************************************

#ifndef INC_DISPLAY_TASK_H_
#define INC_DISPLAY_TASK_H_

//******************************* Include Files *******************************

//******************************* Global Types ********************************

//***************************** Global Constants ******************************

typedef enum{
	CMD_ACK,
	CMD_ACCELEROMETER_DATA,
	CMD_COMPASS_DATA,
	CMD_CHANGE_MODE
}CMD_TYPE;

//***************************** Global Variables ******************************

//***************************** Function Declaration **************************
void DisplayTask();

//*********************** Inline Method Implementations ***********************

#endif /* INC_DISPLAY_TASK_H_ */
