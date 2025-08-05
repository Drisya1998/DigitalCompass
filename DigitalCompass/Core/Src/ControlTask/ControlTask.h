//**************************** ControlTask ************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// Summary : ControlTask header File
// Note    : Added Macros
//
//*****************************************************************************

#ifndef INC_CONTROL_TASK_H_
#define INC_CONTROL_TASK_H_

//******************************* Include Files *******************************

//******************************* Global Types ********************************

//***************************** Global Constants ******************************
#define CHANGE_MODE 				0x03
#define CONTROL_PAYLOAD_LENGTH		6
#define CHECKSUM					11
//***************************** Global Variables ******************************

//***************************** Function Declaration **************************
void ControlTask();

//*********************** Inline Method Implementations ***********************

#endif /* INC_CONTROL_TASK_H_ */
