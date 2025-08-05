//*******************************AppMain***************************************
//Copyright (c) 2025 Trenser Technology Solutions
//All Rights Reserved
//*****************************************************************************
//
//File     : AppMain.c
//Summary  : Digital compass Function Tasks are Initialized
//Note     : None
//Author   : Drisya P
//Date     : 25/Jul/2025
//
//*****************************************************************************

//*********************Include Files*******************************************
#include <stdio.h>
#include <stdbool.h>
#include "AppMain.h"
#include "osTask.h"
//*********************Local Types*********************************************

//*********************Local Constants*****************************************

//*********************Local Variables*****************************************

//*********************Local Functions*****************************************

//*********************.AppMain.***********************************************
//Purpose : Digital compass Function Tasks are Initialized
//Inputs  : None
//Outputs : None
//Return  : None
//Notes   : None
//*****************************************************************************
void AppMain()
{
	bool blFlag =  FALSE;

	printf("Hello from STM32,...........DIGITAL COMPASS.............!\r\n");

	blFlag = TaskInit();

	if(blFlag == FALSE)
	{
		printf("Thread Creation Failed\r\n");
	}
}

//EOF
