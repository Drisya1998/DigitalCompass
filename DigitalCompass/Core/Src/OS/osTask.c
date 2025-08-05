//*******************************OSTask*****************************************
//Copyright (c) 2025 Trenser Technology Solutions
//All Rights Reserved
//*****************************************************************************
//
//File     : osTask.c
//Summary  : Handles Thread Operations.
//Note     : None
//Author   : Drisya P
//Date     : 25/Jul/2025
//
//*****************************************************************************

//*********************Include Files*******************************************
#include <osTask.h>
#include <stdio.h>
#include <stdbool.h>
#include "AppMain.h"
#include "cmsis_os2.h"
#include "stm32f1xx_hal.h"
#include "ControlTask.h"
#include "SensorTask.h"
#include "DisplayTask.h"

//*********************Local Types*********************************************

//*********************Local Constants*****************************************

//*********************Local Variables*****************************************
static osThreadId_t ThreadHandles[TASKS_MAX_SIZE] = {0, 0, 0};
static uint8 ucThreadIndex = 0;
osMutexId_t uartMutex = NULL;

TASKS stTasks[TASKS_MAX_SIZE] =
	{
		{(uint8*)"Control", 0, STACK_SIZE, THREAD_PRIORITY, ControlTask},
		{(uint8*)"Sensor", 0, STACK_SIZE, THREAD_PRIORITY, SensorTask},
		{(uint8*)"Display", 0, STACK_SIZE, THREAD_PRIORITY, DisplayTask}
	};

//*********************Local Functions*****************************************
static bool osTaskCreate();
static bool CreateUARTMutex();

//*********************.TaskInit.**********************************************
//Purpose :	Initialize Each Tasks and Create Mutex.
//Inputs  : None
//Outputs : None
//Return  : TRUE - initialized , FALSE - Error
//Notes   : None
//*****************************************************************************
bool TaskInit()
{
	uint8 ucIndex = 0;
	bool blFlag = FALSE;

	CreateUARTMutex();

	for(ucIndex = 0; ucIndex < TASKS_MAX_SIZE; ucIndex++)
	{
		blFlag = osTaskCreate(&stTasks[ucIndex]);
	}

	return blFlag;
}

//*********************.osTaskCreate.******************************************
//Purpose :	Create each task
//Inputs  : stTask - structure include Thread name , stack size , priority ,
//				and	the function executed by thread.
//Outputs : None
//Return  : TRUE - created thread successfully , FALSE - error
//Notes   : None
//*****************************************************************************
static bool osTaskCreate(TASKS *stTask)
{
	bool blFlag = FALSE;
	osThreadAttr_t attr = {0, 0, 0, 0, 0, 0, 0, 0, 0};

	if(stTask != NULL)
	{
		attr.name = (const char *)stTask->pucTaskName;
		attr.stack_size = stTask->ulStackSize;
		attr.priority = (osPriority_t)osPriorityNormal;

		ThreadHandles[ucThreadIndex] = osThreadNew(stTask->pTaskFunc,
														NULL, &attr);
		stTask->Thread_Id = ThreadHandles[ucThreadIndex];

		if(ThreadHandles[ucThreadIndex] == NULL)
		{
			blFlag = FALSE;
		    printf("Failed to create thread: %s\r\n", stTask->pucTaskName);
		}
		else
		{
			printf("Created thread: %s\r\n", stTask->pucTaskName);
			ucThreadIndex++;
			blFlag = TRUE;
		}

	}

	return blFlag;
}

//*********************.CreateUARTMutex.***************************************
//Purpose : Create Mutex
//Inputs  : None
//Outputs : None
//Return  : TRUE - return success , FALSE - Failed
//Notes   : None
//*****************************************************************************
static bool CreateUARTMutex()
{
	bool blFlag = FALSE;

	uartMutex = osMutexNew(NULL);

	if(uartMutex != NULL)
	{
		blFlag = TRUE;
	}

	return blFlag;
}


//*********************.UARTMutexRelease.**************************************
//Purpose : unlock Mutex
//Inputs  : None
//Outputs : None
//Return  : TRUE - return success , FALSE - Failed
//Notes   : None
//*****************************************************************************
bool UARTMutexRelease()
{
	bool blFlag = FALSE;

	if(osMutexRelease(uartMutex) == osOK)
	{
		blFlag = TRUE;
	}

	return blFlag;
}

//*********************.UARTMutexAcquire.**************************************
//Purpose : lock Mutex
//Inputs  : None
//Outputs : None
//Return  : TRUE - return success , FALSE - Failed
//Notes   : None
//*****************************************************************************
bool UARTMutexAcquire()
{
	bool blFlag = FALSE;

	if(osMutexAcquire(uartMutex, osWaitForever) == osOK)
	{
		blFlag = TRUE;
	}

	return blFlag;
}

//*********************.osTaskDelay.*******************************************
//Purpose : To set delay for Tasks
//Inputs  : delay - delay in milliseconds
//Outputs : None
//Return  : None
//Notes   : None
//*****************************************************************************
void osTaskDelay(uint32 delay)
{
	osDelay(delay);
}

//EOF
