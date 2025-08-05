//*******************************i2c_hal***************************************
//Copyright (c) 2025 Trenser Technology Solutions
//All Rights Reserved
//*****************************************************************************
//
//File     : i2c_hal.c
//Summary  : I2C Interface
//Note     : None
//Author   : Drisya P
//Date     : 29/Jul/2025
//
//*****************************************************************************

//*********************Include Files*******************************************
#include <stdbool.h>
#include "AppMain.h"
#include "i2c_hal.h"
#include "main.h"
#include "stm32f1xx_hal_i2c.h"
#include <stdio.h>
#include "osTask.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include <math.h>

//*********************Local Types*********************************************

//*********************Local Constants*****************************************

//*********************Local Variables*****************************************
extern I2C_HandleTypeDef hi2c1;
//static uint8 pucDisplayBuffer[DISPLAY_WIDTH][DISPLAY_PAGES] = {0};
static COORDINATES gstCoordinate = {0, 0, 0};
volatile DISPLAYMODE eCurrentI2CDisplayMode = DISPLAY_MODE_DATA;
static uint8 gucPrevXend = CENTER_X;
static uint8 gucPrevYend = CENTER_Y - RADIUS;


//*********************Local Functions*****************************************
static void i2cHalDrawCompass();
static void i2cHalDrawData();
static void i2cHalDrawNeedle(uint16);
//*********************.i2cHalIdentifyDevice.**********************************
//Purpose : To Read the Data in Identifier Register A
//Inputs  : pucDataBuffer - address of Data Buffer
//Outputs : None
//Return  : TRUE - Read successfully , FALSE - Failed
//Notes   : None
//*****************************************************************************
bool i2cHalIdentifyDevice(uint8* pucDeviceChipId)
{
	bool blFlag = FALSE;

	if(pucDeviceChipId != NULL)
	{
		if(HAL_I2C_IsDeviceReady(&hi2c1, HMC5883L_I2C_ADDR, 2, 100) != HAL_OK)
		{
			LOG("I2C ACCELEROMETER Device Not Ready\r\n");
		}

		if(HAL_I2C_Mem_Read(&hi2c1, HMC5883L_I2C_ADDR,
					HMC5883L_CHIP_ID_REG, I2C_MEMADD_SIZE_8BIT,
			pucDeviceChipId, HMC5883L_BUFFER_LEN, HAL_MAX_DELAY) == HAL_OK)
		{
			blFlag = TRUE;
		}
	}
	//TEST CODE
	/*uint8 ucRegAddr = 0x0B;
	uint8 ucRegData = 0;
	uint8 pucBuffer1[2] = {0x0B, 0xF0};
	uint8 pucBuffer2[2] = {0x0B, 0x00};*/

	/*for (uint8 ucAddr = 1; ucAddr < 127; ucAddr++)
	 {
	    if (HAL_I2C_IsDeviceReady(&hi2c1, ucAddr << 1, 2, 10) == HAL_OK)
	    {
	        printf("Found device at: 0x%02X\r\n", ucAddr << 1);
	    }
	}*/

	/*if(HAL_I2C_Master_Transmit(&hi2c1, HMC5883L_I2C_ADDR, &ucRegAddr,
		1, HAL_MAX_DELAY) == HAL_OK)
	{
		if(HAL_I2C_Master_Receive(&hi2c1, HMC5883L_I2C_ADDR,
				&ucRegData, 1, HAL_MAX_DELAY) == HAL_OK)
		{
			LOG("INITIAL SET/RESET PERIOD REG: 0x%02X\r\n", ucRegData);

			if(HAL_I2C_Master_Transmit(&hi2c1, HMC5883L_I2C_ADDR, pucBuffer1,
							2, HAL_MAX_DELAY) == HAL_OK)
			{
				LOG("WRITTEN TO REG 0x0B\r\n");

				if(HAL_I2C_Master_Transmit(&hi2c1, HMC5883L_I2C_ADDR,
				 &ucRegAddr, 1, HAL_MAX_DELAY) == HAL_OK)
				{
					if(HAL_I2C_Master_Receive(&hi2c1, HMC5883L_I2C_ADDR,
							&ucRegData, 1, HAL_MAX_DELAY) == HAL_OK)
					{
						LOG("FINAL PERIOD REG: 0x%02X\r\n", ucRegData);

						if(HAL_I2C_Master_Transmit(&hi2c1, HMC5883L_I2C_ADDR,
									pucBuffer2, 2, HAL_MAX_DELAY) == HAL_OK)
						{
							LOG("WRITTEN TO REG 0x0B\r\n");
						}
					}
				}
			}
		}
	}*/

	return blFlag;
}

//*********************.i2cHalDrawNeedle.**************************************
//Purpose : To draw Needle
//Inputs  : unAngle - angle
//Outputs : None
//Return  : TRUE - Draw successfully , FALSE - Failed
//Notes   : None
//*****************************************************************************
static void i2cHalDrawNeedle(uint16 unAngle)
{
	uint8 ucXend = 0;
	uint8 ucYend = 0;
	uint8 pucBuffer[6] ={0};
	float fRad = 0.0;

	LOG("I2C Display- Angle - %d\r\n", unAngle);
	fRad = unAngle * (M_PI / 180.0f);;
	ucXend = CENTER_X + RADIUS * sinf(fRad);
	ucYend = CENTER_Y - RADIUS * cosf(fRad);

	ssd1306_DrawLine(CENTER_X, CENTER_Y, gucPrevXend, gucPrevYend, Black);
	ssd1306_DrawLine(CENTER_X, CENTER_Y, ucXend, ucYend, White);

	ssd1306_FillRectangle(0, 10, 45, 25, Black);
	ssd1306_SetCursor(0, 10);
	sprintf((char*)pucBuffer, "%d", unAngle);
	ssd1306_WriteString((char*)pucBuffer, Font_11x18, White);
	ssd1306_UpdateScreen();
	gucPrevXend = ucXend;
	gucPrevYend = ucYend;
}
//*********************.i2cHalDrawCompass.*************************************
//Purpose : To draw compass in Display
//Inputs  : None
//Outputs : None
//Return  : TRUE - Draw successfully , FALSE - Failed
//Notes   : None
//*****************************************************************************
static void i2cHalDrawCompass()
{
	uint16 unAngle = 0;
	uint8 ucX = 0;
	uint8 ucY = 0;

	ssd1306_Fill(Black);

	for (unAngle = 0; unAngle < 360; unAngle += 5)
	{
		ucX = CENTER_X + RADIUS * cos(unAngle * M_PI / 180);
		ucY = CENTER_Y + RADIUS * sin(unAngle * M_PI / 180);
	    ssd1306_DrawPixel(ucX, ucY, White);
	}

	ssd1306_DrawLine(CENTER_X, CENTER_Y, CENTER_X, CENTER_Y - RADIUS, White);

	gucPrevXend = CENTER_X;
	gucPrevYend = CENTER_Y - RADIUS;

	ssd1306_SetCursor(CENTER_X - 3, CENTER_Y - RADIUS - 10);
	ssd1306_WriteString("N", Font_6x8, White);

	ssd1306_SetCursor(CENTER_X - 3, CENTER_Y + RADIUS + 2);
	ssd1306_WriteString("S", Font_6x8, White);

	ssd1306_SetCursor(CENTER_X - RADIUS - 8, CENTER_Y - 3);
	ssd1306_WriteString("W", Font_6x8, White);

	ssd1306_SetCursor(CENTER_X + RADIUS + 2, CENTER_Y - 3);
	ssd1306_WriteString("E", Font_6x8, White);

	ssd1306_UpdateScreen();
}

//*********************.i2cHalDrawData.*************************************
//Purpose : To draw in Display
//Inputs  : None
//Outputs : None
//Return  : TRUE - Draw successfully , FALSE - Failed
//Notes   : None
//*****************************************************************************
static void i2cHalDrawData()
{
	uint8 pucBuffer[16] = {0};

	ssd1306_Fill(Black);

	ssd1306_SetCursor(0, 10);
	ssd1306_WriteString("X:", Font_11x18, White);
	sprintf((char*)pucBuffer, "%d", gstCoordinate.nX);
	ssd1306_WriteString((char*)pucBuffer, Font_11x18, White);

	ssd1306_SetCursor(55, 10);
	ssd1306_WriteString("Y:", Font_11x18, White);
	sprintf((char*)pucBuffer, "%d", gstCoordinate.nY);
	ssd1306_WriteString((char*)pucBuffer, Font_11x18, White);

	ssd1306_SetCursor(30, 40);
	ssd1306_WriteString("Z:", Font_11x18, White);
	sprintf((char*)pucBuffer, "%d", gstCoordinate.nZ);
	ssd1306_WriteString((char*)pucBuffer, Font_11x18, White);

	ssd1306_UpdateScreen();
}


//DRIVER CODE
//*********************.i2cHalDraw.********************************************
//Purpose : To draw in Display
//Inputs  : ucX - x coordinate , ucY- y coordinate
//Outputs : None
//Return  : TRUE - Draw successfully , FALSE - Failed
//Notes   : None
//*****************************************************************************
/*bool i2cHalDraw(uint8 ucX, uint8 ucY, uint8 ucPage)
{
	bool blFlag =  FALSE;
	uint8 ucPageAddress = 0xB0 | ucPage;
	uint8 ucPixelByte = 0;

	if((ucX < DISPLAY_WIDTH) && (ucPage < DISPLAY_PAGES) &&
			(ucY < DISPLAY_PAGES))
	{
		pucDisplayBuffer[ucX][ucPage] |= (1 << ucY);

		ucPixelByte = pucDisplayBuffer[ucX][ucPage];

		HAL_I2C_Master_Transmit(&hi2c1, DISPLAY_I2C_ADDR,
					(uint8[]){0x00, 0x20, 0x00}, 3, HAL_MAX_DELAY);
		HAL_I2C_Master_Transmit(&hi2c1, DISPLAY_I2C_ADDR,
					(uint8[]){0x00, ucPageAddress}, 2, HAL_MAX_DELAY);
		HAL_I2C_Master_Transmit(&hi2c1, DISPLAY_I2C_ADDR,
					(uint8[]){0x00, 0x00 | (ucX & 0x0F)}, 2, HAL_MAX_DELAY);
		HAL_I2C_Master_Transmit(&hi2c1, DISPLAY_I2C_ADDR,
					(uint8_t[]){0x00, 0x10 | (ucX >> 4)}, 2, HAL_MAX_DELAY);
		if((HAL_I2C_Master_Transmit(&hi2c1, DISPLAY_I2C_ADDR,
				(uint8[]){0x40, ucPixelByte}, 2, HAL_MAX_DELAY))== HAL_OK)
		{
			blFlag = TRUE;
		}
	}

	return blFlag;
}*/

//*********************.i2cHalDisplayInit.*************************************
//Purpose : To Init the i2c Display
//Inputs  : None
//Outputs : None
//Return  : TRUE - Initialized , FALSE - Failed
//Notes   : None
//*****************************************************************************
bool i2cHalDisplayInit()
{
	bool blFlag = FALSE;

	//DRIVER CODE
	/*if(HAL_I2C_IsDeviceReady(&hi2c1, DISPLAY_I2C_ADDR, 2, 100) == HAL_OK)
	{
		HAL_I2C_Master_Transmit(&hi2c1, DISPLAY_I2C_ADDR,
				(uint8[]){0x00, 0xA8, 0x3F}, 3, HAL_MAX_DELAY);
		HAL_I2C_Master_Transmit(&hi2c1, DISPLAY_I2C_ADDR,
				(uint8[]){0x00, 0xD3, 0x00}, 3, HAL_MAX_DELAY);
		HAL_I2C_Master_Transmit(&hi2c1, DISPLAY_I2C_ADDR,
				(uint8[]){0x00, 0x40}, 2, HAL_MAX_DELAY);
		HAL_I2C_Master_Transmit(&hi2c1, DISPLAY_I2C_ADDR,
				(uint8[]){0x00, 0xA0}, 2, HAL_MAX_DELAY);
		HAL_I2C_Master_Transmit(&hi2c1, DISPLAY_I2C_ADDR,
				(uint8[]){0x00, 0xC8}, 2, HAL_MAX_DELAY);
		HAL_I2C_Master_Transmit(&hi2c1, DISPLAY_I2C_ADDR,
				(uint8[]){0x00, 0xDA, 0x02}, 3, HAL_MAX_DELAY);
		HAL_I2C_Master_Transmit(&hi2c1, DISPLAY_I2C_ADDR,
				(uint8[]){0x00, 0x81, 0x7F}, 3, HAL_MAX_DELAY);
		HAL_I2C_Master_Transmit(&hi2c1, DISPLAY_I2C_ADDR,
				(uint8[]){0x00, 0xA4}, 2, HAL_MAX_DELAY);
		HAL_I2C_Master_Transmit(&hi2c1, DISPLAY_I2C_ADDR,
				(uint8[]){0x00, 0xA6}, 2, HAL_MAX_DELAY);
		HAL_I2C_Master_Transmit(&hi2c1, DISPLAY_I2C_ADDR,
				(uint8[]){0x00, 0xD5, 0x80}, 3, HAL_MAX_DELAY);
		HAL_I2C_Master_Transmit(&hi2c1, DISPLAY_I2C_ADDR,
				(uint8[]){0x00, 0x8D, 0x14}, 3, HAL_MAX_DELAY);

		for (uint8 ucPage = 0; ucPage < 8; ucPage++)
		{
		    HAL_I2C_Master_Transmit(&hi2c1, DISPLAY_I2C_ADDR,
		        (uint8[]){0x00, (uint8)(0xB0 | ucPage)}, 2, HAL_MAX_DELAY);
		    HAL_I2C_Master_Transmit(&hi2c1, DISPLAY_I2C_ADDR,
		        (uint8[]){0x00, 0x00}, 2, HAL_MAX_DELAY);
		    HAL_I2C_Master_Transmit(&hi2c1, DISPLAY_I2C_ADDR,
		        (uint8[]){0x00, 0x10}, 2, HAL_MAX_DELAY);

		    for (uint8 ucCol = 0; ucCol < 128; ucCol++)
		    {
		        HAL_I2C_Master_Transmit(&hi2c1, DISPLAY_I2C_ADDR,
		            (uint8[]){0x40, 0x00}, 2, HAL_MAX_DELAY);
		    }
		}
		blFlag = TRUE;
	}*/

	ssd1306_Init();
	ssd1306_Fill(Black);
	ssd1306_SetCursor(10, 10);
	ssd1306_WriteString("Hello", Font_11x18, White);
	ssd1306_UpdateScreen();
	osTaskDelay(DELAY_2000);
	blFlag = TRUE;

	return  blFlag;
}

//*********************.i2cHalChangeDisplayMode.*******************************
//Purpose : To Change the Display Mode
//Inputs  : None
//Outputs : None
//Return  : TRUE - Changed successfully , FALSE - Failed
//Notes   : None
//*****************************************************************************
bool i2cHalChangeDisplayMode(DISPLAYMODE eCurrentDisplayMode)
{
	bool blFlag = FALSE;

	eCurrentI2CDisplayMode = eCurrentDisplayMode;

	if (eCurrentDisplayMode == DISPLAY_MODE_COMPASS)
	{
		blFlag = TRUE;
		i2cHalDrawCompass();
	}
	else
	{
		i2cHalDrawData();
		blFlag = TRUE;
	}

	return blFlag;
}

//*********************.i2cHalReadCoordinate.**********************************
//Purpose : To Read X, Y, Z coordinate from AcceleroMeter
//Inputs  : None
//Outputs : None
//Return  : TRUE - Read successfully , FALSE - Failed
//Notes   : None
//*****************************************************************************
bool i2cHalReadCoordinate(COORDINATES* stCoordinate)
{
	bool blFlag = FALSE;
	uint8 ucRawData[6] = {0, 0, 0, 0, 0, 0};

	if(HAL_I2C_Mem_Read(&hi2c1, HMC5883L_I2C_ADDR, HMC5883L_XLSB_REG,
			I2C_MEMADD_SIZE_8BIT, ucRawData, COORDINATES_LENGTH,
			HAL_MAX_DELAY) == HAL_OK)
	{
		stCoordinate->nX = (int16)(ucRawData[1] << 8 | ucRawData[0]);
		stCoordinate->nY = (int16)(ucRawData[3] << 8 | ucRawData[2]);
		stCoordinate->nZ = (int16)(ucRawData[5] << 8 | ucRawData[4]);
		blFlag = TRUE;
	}

	return blFlag;
}

//*********************.i2cHalDisplayCoordinates.******************************
//Purpose : To Display X, Y, Z coordinate from AcceleroMeter
//Inputs  : stCoordinate -  Coordinates
//Outputs : None
//Return  : TRUE - Displayed successfully , FALSE - Failed
//Notes   : None
//*****************************************************************************
bool i2cHalDisplayCoordinates(COORDINATES stCoordinate)
{
	bool blFlag = FALSE;

	gstCoordinate.nX = stCoordinate.nX;
	gstCoordinate.nY = stCoordinate.nY;
	gstCoordinate.nZ = stCoordinate.nZ;

	LOG("I2C Display ---> X=0x%04X Y=0x%04X Z=0x%04X\r\n", gstCoordinate.nX,
			gstCoordinate.nY, gstCoordinate.nZ);
	if(eCurrentI2CDisplayMode == DISPLAY_MODE_DATA)
	{
		i2cHalDrawData();
	}

	blFlag = TRUE;
	return blFlag;
}

//*********************.i2cHalChangeNeedlePosition.****************************
//Purpose : To Change  Needle Position according to the angle.
//Inputs  : unAngle - angle
//Outputs : None
//Return  : TRUE - Changed successfully , FALSE - Failed
//Notes   : None
//*****************************************************************************
bool i2cHalChangeNeedlePosition(uint16 unAngle)
{
	bool blFlag = FALSE;

	if(eCurrentI2CDisplayMode == DISPLAY_MODE_COMPASS)
	{
		i2cHalDrawNeedle(unAngle);
	}

	return blFlag;
}

//*********************.i2cHalInitializeDevice.*********************************
//Purpose : To Initialize AcceleroMeter
//Inputs  : None
//Outputs : None
//Return  : TRUE - Initialized successfully , FALSE - Failed
//Notes   : None
//*****************************************************************************
bool i2cHalInitializeDevice()
{
	bool blFlag = FALSE;
	uint8 ucConfig = 0x1D;

	if(HAL_I2C_Mem_Write(&hi2c1, HMC5883L_I2C_ADDR, HMC5883L_CONTROL_REG,
	               I2C_MEMADD_SIZE_8BIT, &ucConfig, 1, HAL_MAX_DELAY) == HAL_OK)
	{
		blFlag = TRUE;
	}

	return blFlag;
}

//EOF
