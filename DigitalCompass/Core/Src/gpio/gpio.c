//*******************************gpio******************************************
//Copyright (c) 2025 Trenser Technology Solutions
//All Rights Reserved
//*****************************************************************************
//
//File     : gpio.c
//Summary  : gpio Interface
//Note     : None
//Author   : Drisya P
//Date     : 25/Jul/2025
//
//*****************************************************************************

//*********************Include Files*******************************************
#include <stdbool.h>
#include "AppMain.h"
#include "gpio.h"
#include "main.h"

//*********************Local Types*********************************************

//*********************Local Constants*****************************************

//*********************Local Variables*****************************************

//*********************Local Functions*****************************************

//*********************.gpioReadButtonPress.***********************************
//Purpose : To Read the Button press
//Inputs  : None
//Outputs : None
//Return  : TRUE - Button Pressed , FALSE - Button Not pressed
//Notes   : None
//*****************************************************************************
bool gpioReadButtonPress()
{
	bool blFlag = FALSE;

	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
	{
		blFlag = TRUE;
	}

	return blFlag;
}

//EOF
