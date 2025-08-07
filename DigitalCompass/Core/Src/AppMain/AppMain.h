//**************************** appMain ****************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// Summary : AppMain header File
// Note    : Added Macros, Datatypes and Message Protocols
//
//*****************************************************************************

#ifndef INC_APPMAIN_H_
#define INC_APPMAIN_H_

//******************************* Include Files *******************************
#include <stdbool.h>

//******************************* Global Types ********************************
typedef unsigned short uint16;
typedef signed short int16;
typedef signed char int8;
typedef unsigned char uint8;
typedef unsigned long uint32;

typedef struct _REQUEST_MSG_
{
    uint16 unUId;
    uint8 ucCmd;
    uint16 unLength;
    int16* punPayLoad;
    uint8 ucCheckSum;
}REQUEST_MSG;

typedef struct _ACK_MSG_
{
    uint32 ulUId;
    uint8 ucCmd;
    uint8 ucState;
}ACK_MSG;

//***************************** Global Constants ******************************
#define TRUE                1
#define FALSE               0
#define DELAY_200			200
#define DELAY_100			100
#define DELAY_50			50
#define DELAY_500			500
#define DELAY_2000			2000
#define ACK_STATUS_OK 		0x00
#define ACK_STAUS_ERROR		0x01

#define LOG(...)               					\
    do {                       					\
        if(UARTMutexAcquire())    				\
		{										\
        	printf(__VA_ARGS__);   				\
        }										\
        UARTMutexRelease();    					\
    } while (0)

//***************************** Global Variables ******************************

//***************************** Function Declaration **************************
void AppMain();

//*********************** Inline Method Implementations ***********************

#endif /* INC_APPMAIN_H_ */
