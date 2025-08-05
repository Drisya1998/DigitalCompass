//**************************** i2c_hal ****************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// Summary : i2c_hal header File
// Note    : Added Function declaration
//
//*****************************************************************************

#ifndef INC_I2C_HAL_H_
#define INC_I2C_HAL_H_
//******************************* Include Files *******************************
#include "AppMain.h"

//******************************* Global Types ********************************
typedef enum {
    DISPLAY_MODE_COMPASS,
    DISPLAY_MODE_DATA
}DISPLAYMODE;

typedef struct _COORDINATES_
{
	int16 nX;
	int16 nY;
	int16 nZ;
}COORDINATES;
//***************************** Global Constants ******************************
#define HMC5883L_I2C_ADDR 		(0x0D << 1)
#define HMC5883L_CHIP_ID_REG	(0x0D)
#define HMC5883L_CONTROL_REG	(0x09)
#define HMC5883L_XLSB_REG		(0x00)
#define DISPLAY_I2C_ADDR		(0x3C << 1)
#define HMC5883L_BUFFER_LEN		1
#define DISPLAY_WIDTH 			128
#define DISPLAY_PAGES 			8
#define DISPLAY_HEIGHT 			64
#define CENTER_X 				(DISPLAY_WIDTH / 2)
#define CENTER_Y 				(DISPLAY_HEIGHT / 2)
#define RADIUS   				20
#define COORDINATES_LENGTH		6
//***************************** Global Variables ******************************

//***************************** Function Declaration **************************
bool i2cHalIdentifyDevice(uint8*);
bool i2cHalDisplayInit();
bool i2cHalChangeDisplayMode(DISPLAYMODE);
bool i2cHalReadCoordinate();
bool i2cHalInitializeDevice();
bool i2cHalChangeNeedlePosition(uint16);
bool i2cHalDisplayCoordinates();
//*********************** Inline Method Implementations ***********************

#endif /* INC_I2C_HAL_H_ */
