//*******************************SensorTask************************************
//Copyright (c) 2025 Trenser Technology Solutions
//All Rights Reserved
//*****************************************************************************
//
//File     : SensorTask.c
//Summary  : Read AcceleroMeter Data and compass Data from AcceleroMeter via
//           I2C
//Note     : None
//Author   : Drisya P
//Date     : 29/Jul/2025
//
//*****************************************************************************

//*********************Include Files*******************************************
#include <stdio.h>
#include <stdbool.h>
#include "SensorTask.h"
#include "gpio.h"
#include "AppMain.h"
#include "osMsgq.h"
#include "osTask.h"
#include "i2c_hal.h"
#include <math.h>

//*********************Local Types*********************************************

//*********************Local Constants*****************************************

//*********************Local Variables*****************************************
static uint16 sgunUIdCounter = 0;
static int16 gpunPayLoad[3] = {0x0000, 0x0000, 0x0000};
static uint16 gunAngle = 0;
static uint8 gucCurrentSensorData = ACCELEROMETER_DATA;

//*********************Local Functions*****************************************
static bool SensorTaskBuildRequest(COORDINATES*, REQUEST_MSG*);
static bool SensorTaskReadCoordinates(COORDINATES*);
//*********************.SensorTask.********************************************
//Purpose :	Read data via I2C
//Inputs  : None
//Outputs : None
//Return  : None
//Notes   : None
//*****************************************************************************
void SensorTask()
{
	uint8 ucDeviceChipId = 0;
	REQUEST_MSG stReqMsg = {0, 0, 0, 0, 0};
	ACK_MSG stAckMsg = {0, 0, 0};
	COORDINATES stCoordinate = {0, 0, 0};

	if(i2cHalIdentifyDevice(&ucDeviceChipId))
	{
		LOG("HMC5883L CHIP ID REG VALUE-->%02X \r\n", ucDeviceChipId);
	}

	if(osMsgqDisplayToSensorInit(sizeof(stAckMsg)))
	{
		while(1){
			if(SensorTaskReadCoordinates(&stCoordinate))
			{
				LOG("Sensor Task- Requist Building\r\n");
				if(SensorTaskBuildRequest(&stCoordinate, &stReqMsg))
				{
					if(osMsgqMessageSendToDisplay(stReqMsg))
					{
						osTaskDelay(DELAY_200);
					}
					if(osMsgqMessageRcvInSensor(&stAckMsg))
					{
						LOG("Sensor Task: ACKUID=%lu, CMD=0x%02X, "
									"STATE=0x%02X\r\n\n", \
								stAckMsg.ulUId, stAckMsg.ucCmd,
										stAckMsg.ucState);
					}
				}
			}

			osTaskDelay(DELAY_500);
		}
	}
}

//*********************.SensorTaskBuildRequest.********************************
//Purpose :	Build the Request with X, Y, Z coordinate values..
//Inputs  : stReqMsg -  Request Message
//Outputs : None
//Return  : TRUE - SUCCESS , FALSE - Failed
//Notes   : None
//*****************************************************************************
static bool SensorTaskBuildRequest(COORDINATES* stCoordinate,
											REQUEST_MSG* stReqMsg)
{
	bool blFlag = FALSE;

	if((stReqMsg != NULL) && (stCoordinate != NULL))
	{
		stReqMsg->unUId = ++sgunUIdCounter;
		stReqMsg->unLength = PAYLOAD_LENGTH;
		stReqMsg->ucCheckSum = SENSOR_CHECKSUM;
		if(gucCurrentSensorData == ACCELEROMETER_DATA)
		{
			stReqMsg->ucCmd = ACCELEROMETER_DATA;
			gpunPayLoad[0] = stCoordinate->nX;
			gpunPayLoad[1] = stCoordinate->nY;
			gpunPayLoad[2] = stCoordinate->nZ;
			stReqMsg->punPayLoad = gpunPayLoad;
			stReqMsg->ucCheckSum = SENSOR_CHECKSUM;
			gucCurrentSensorData = COMPASS_DATA;
		}
		else
		{
			gpunPayLoad[0] = gunAngle;
			gpunPayLoad[1] = 0;
			gpunPayLoad[2] = 0;
			stReqMsg->ucCmd = COMPASS_DATA;
			stReqMsg->punPayLoad = gpunPayLoad;
			gucCurrentSensorData = ACCELEROMETER_DATA;
		}

		blFlag = TRUE;
	}

	return blFlag;
}

//*********************.SensorTaskReadCoordinates.*****************************
//Purpose :	Read  X, Y, Z coordinate values..
//Inputs  : stCoordinate - Coordinates structure
//Outputs : None
//Return  : TRUE - SUCCESS , FALSE - Failed
//Notes   : None
//*****************************************************************************
static bool SensorTaskReadCoordinates(COORDINATES* stCoordinate)
{
	bool blFlag = FALSE;
	float fAngle = 0.0;
	float fOffsetX = 0.0;
	float fOffsetY = 0.0;
	float fCorrectedX = 0.0;
	float fCorrectedY = 0.0;
	float fScaleX = 0.0;
	float fScaleY = 0.0;
	float fNormX = 0.0;
	float fNormY = 0.0;

	if(stCoordinate != NULL)
	{
		do{
			if(!i2cHalInitializeDevice())
			{
				break;
			}

			if(i2cHalReadCoordinate(stCoordinate))
			{
				fOffsetX = (1621 + 1087) / 2.0f;
				fOffsetY = (2417 + 1912) / 2.0f;

				fScaleX = (1621 - 1087) / 2.0f;
				fScaleY = (2417 - 1912) / 2.0f;

				fCorrectedX = (float)stCoordinate->nX - fOffsetX;
				fCorrectedY = (float)stCoordinate->nY - fOffsetY;

				fNormX = fCorrectedX / fScaleX;
				fNormY = fCorrectedY / fScaleY;

				fAngle = atan2(fNormY, fNormX) * (180.0 / M_PI);

				if (fAngle < 0.0f)
				{
					fAngle += 360.0f;
				}

				gunAngle = (uint16)fAngle;
				blFlag = TRUE;
			}

		}while(0);
	}

	return blFlag;
}
//EOF
