/*!
 *	\file		ellipseMinimal.c
 *  \author		SBG Systems (Alexis GUINAMARD)
 *	\date		28/03/2014
 *
 *	\brief		C example that simply opens an Ellipse interface.
 *
 *	This small example demonstrates how to initialize the sbgECom library
 *	to read data from an Ellipse using callbacks.
 *
 *	\section CodeCopyright Copyright Notice 
 *	Copyright (C) 2007-2014, SBG Systems SAS. All rights reserved.
 *	
 *	This source code is intended for use only by SBG Systems SAS and
 *	those that have explicit written permission to use it from
 *	SBG Systems SAS.
 *	
 *	THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 *	PARTICULAR PURPOSE.
 */

#include <sbgEComLib.h>
#include <time.h>
#include <share.h>

int count = 0; // untuk count frekuensi
int point = 1;
int countIMU = 200;
int countGPS = 200;
int rateIMU = 6; // 200/33ish
int rateGPS = 40; // 200/5=40
double positionIMU[3];
double positionGPS[3];
float positionIMUStdDev[3];
float positionGPSStdDev[3];
FILE* dataINS;
FILE* dataGPS;
FILE* dataIMU;
FILE* dataGPSmap;
FILE* dataIMUmap;
clock_t t;
double time_spent;

//----------------------------------------------------------------------//
//  Call backs                                                          //
//----------------------------------------------------------------------//

/*!
 *	Callback definition called each time a new log is received.
 *	\param[in]	pHandle									Valid handle on the sbgECom instance that has called this callback.
 *	\param[in]	msgClass								Class of the message we have received
 *	\param[in]	msg										Message ID of the log received.
 *	\param[in]	pLogData								Contains the received log data as an union.
 *	\param[in]	pUserArg								Optional user supplied argument.
 *	\return												SBG_NO_ERROR if the received log has been used successfully.
 */
SbgErrorCode onLogReceivedDefault(SbgEComHandle *pHandle, SbgEComClass msgClass, SbgEComMsgId msg, const SbgBinaryLogData *pLogData, void *pUserArg)
{
	//
	// Handle separately each received data according to the log ID
	//
	switch (msg)
	{
	case SBG_ECOM_LOG_EKF_EULER:
		//
		// Simply display euler angles in real time
		//
		printf("Euler Angles: %3.1f\t%3.1f\t%3.1f\tStd Dev:%3.1f\t%3.1f\t%3.1f \r", 
				sbgRadToDegF(pLogData->ekfEulerData.euler[0]), sbgRadToDegF(pLogData->ekfEulerData.euler[1]), sbgRadToDegF(pLogData->ekfEulerData.euler[2]), 
				sbgRadToDegF(pLogData->ekfEulerData.eulerStdDev[0]), sbgRadToDegF(pLogData->ekfEulerData.eulerStdDev[1]), sbgRadToDegF(pLogData->ekfEulerData.eulerStdDev[2]));
		break;
	default:
		break;
	}
	
	return SBG_NO_ERROR;
}

SbgErrorCode onLogReceivedTest(SbgEComHandle* pHandle, SbgEComClass msgClass, SbgEComMsgId msg, const SbgBinaryLogData* pLogData, void* pUserArg)
{
	//
	// Handle separately each received data according to the log ID
	//
	switch (msg)
	{
	case SBG_ECOM_LOG_IMU_SHORT:
		//
		// Simply display euler angles in real time
		//
		printf("%d\t%d\t%d\r", pLogData->imuShort.deltaVelocity[0], pLogData->imuShort.deltaVelocity[1], pLogData->imuShort.deltaVelocity[2]);
		break;
	default:
		break;
	}

	return SBG_NO_ERROR;
}

SbgErrorCode onLogReceived(SbgEComHandle* pHandle, SbgEComClass msgClass, SbgEComMsgId msg, const SbgBinaryLogData* pLogData, void* pUserArg)
{
	//
	// Handle separately each received data according to the log ID
	//
	switch (msg)
	{
	case SBG_ECOM_LOG_EKF_NAV:
		// posisi IMU
		positionIMU[0] = pLogData->ekfNavData.position[0];
		positionIMU[1] = pLogData->ekfNavData.position[1];
		positionIMU[2] = pLogData->ekfNavData.position[2];
		positionIMUStdDev[0] = pLogData->ekfNavData.positionStdDev[0];
		positionIMUStdDev[1] = pLogData->ekfNavData.positionStdDev[1];
		positionIMUStdDev[2] = pLogData->ekfNavData.positionStdDev[2];
		break;
	case SBG_ECOM_LOG_GPS1_POS:
		// posisi GPS
		positionGPS[0] = pLogData->gpsPosData.latitude;
		positionGPS[1] = pLogData->gpsPosData.longitude;
		positionGPS[2] = pLogData->gpsPosData.altitude;
		positionGPSStdDev[0] = pLogData->gpsPosData.latitudeAccuracy;
		positionGPSStdDev[1] = pLogData->gpsPosData.longitudeAccuracy;
		positionGPSStdDev[2] = pLogData->gpsPosData.altitudeAccuracy;
		break;
	case SBG_ECOM_LOG_GPS1_VEL:
		break;
	default:
		// SBG_ECOM_LOG_IMU_DATA
		// SBG_ECOM_LOG_FAST_IMU_DATA
		// SBG_ECOM_LOG_IMU_SHORT
		// SBG_ECOM_LOG_EKF_EULER
		// SBG_ECOM_LOG_EKF_QUAT
		// SBG_ECOM_LOG_EKF_NAV
		// SBG_ECOM_LOG_GPS1_VEL
		// SBG_ECOM_LOG_GPS1_POS
		/*
		printf("Velocity: %3.1f\t%3.1f\t%3.1f\t Accuracy: %3.1f\t%3.1f\t%3.1f \r",
			pLogData->ekfNavData.velocity[0], pLogData->ekfNavData.velocity[1], pLogData->ekfNavData.velocity[2],
			pLogData->ekfNavData.velocityStdDev[0], pLogData->ekfNavData.velocityStdDev[1], pLogData->ekfNavData.velocityStdDev[2]);

		printf("Position: %3.1f\t%3.1f\t%3.1f\t Accuracy: %3.1f\t%3.1f\t%3.1f \r",
			pLogData->ekfNavData.position[0], pLogData->ekfNavData.position[1], pLogData->ekfNavData.position[2],
			pLogData->ekfNavData.positionStdDev[0], pLogData->ekfNavData.positionStdDev[1], pLogData->ekfNavData.positionStdDev[2]);

		printf("GPS Position: %3.1f\t%3.1f\t%3.1f\t Accuracy: %3.1f\t%3.1f\t%3.1f \r",
			pLogData->gpsPosData.latitude, pLogData->gpsPosData.longitude, pLogData->gpsPosData.altitude,
			pLogData->gpsPosData.latitudeAccuracy, pLogData->gpsPosData.longitudeAccuracy, pLogData->gpsPosData.altitudeAccuracy);

		printf("GPS Vel: %3.1f\t%3.1f\t%3.1f\t Accuracy: %3.1f\t%3.1f\t%3.1f \r",
			pLogData->gpsVelData.velocity[0], pLogData->gpsVelData.velocity[1], pLogData->gpsVelData.velocity[2],
			pLogData->gpsVelData.velocityAcc[0], pLogData->gpsVelData.velocityAcc[1], pLogData->gpsVelData.velocityAcc[2]);

		printf("Euler Angles: %3.1f\t%3.1f\t%3.1f\tStd Dev:%3.1f\t%3.1f\t%3.1f \r",
			sbgRadToDegF(pLogData->ekfEulerData.euler[0]), sbgRadToDegF(pLogData->ekfEulerData.euler[1]), sbgRadToDegF(pLogData->ekfEulerData.euler[2]),
			sbgRadToDegF(pLogData->ekfEulerData.eulerStdDev[0]), sbgRadToDegF(pLogData->ekfEulerData.eulerStdDev[1]), sbgRadToDegF(pLogData->ekfEulerData.eulerStdDev[2]));

		printf("Accel: %3.1f\t%3.1f\t%3.1f \r",
			pLogData->imuData.accelerometers[0], pLogData->imuData.accelerometers[1], pLogData->imuData.accelerometers[2]);


		*/
		break;
	} // %3.1f\t%3.1f\t%3.1f

	// update time
	t = clock();
	time_spent = ((double)t)/CLOCKS_PER_SEC;

	// print terminal
	printf("%d\t%.3f\t IMU: %f\t%f\t%f\t GPS: %f\t%f\t%f \r", count, time_spent,
		positionIMU[0], positionIMU[1], positionIMU[2],
		positionGPS[0], positionGPS[1], positionGPS[2]);

	// write data ke file
	if (count >= 200) {
		fprintf(dataINS, "%d,%.3f,%f,%f,%f,%.2f,%.2f,%.2f,%f,%f,%f,%.2f,%.2f,%.2f,%.3f,%.3f,%.3f,%.2f,%.2f,%.2f,%.3f,%.3f,%.3f,%.2f,%.2f,%.2f\n", point, time_spent,
			positionIMU[0], positionIMU[1], positionIMU[2],
			positionIMUStdDev[0], positionIMUStdDev[1], positionIMUStdDev[2],
			positionGPS[0], positionGPS[1], positionGPS[2],
			positionGPSStdDev[0], positionGPSStdDev[1], positionGPSStdDev[2],
			pLogData->ekfNavData.velocity[0], pLogData->ekfNavData.velocity[1], pLogData->ekfNavData.velocity[2],
			pLogData->ekfNavData.velocityStdDev[0], pLogData->ekfNavData.velocityStdDev[1], pLogData->ekfNavData.velocityStdDev[2],
			pLogData->gpsVelData.velocity[0], pLogData->gpsVelData.velocity[1], pLogData->gpsVelData.velocity[2],
			pLogData->gpsVelData.velocityAcc[0], pLogData->gpsVelData.velocityAcc[1], pLogData->gpsVelData.velocityAcc[2]);
		point++;
	}
	
	// 
	if (count == countIMU) {
		fprintf(dataIMU, "%d,%.3f,%f,%f,%f\n", count, time_spent,
			positionIMU[0], positionIMU[1], positionIMU[2]);
		fprintf(dataIMUmap, "%f,%f,%f\n", positionIMU[1], positionIMU[0], positionIMU[2]);
		countIMU = countIMU + rateIMU;
	}

	if (count == countGPS) {
		fprintf(dataGPS, "%d,%.3f,%f,%f,%f\n", count, time_spent,
			positionGPS[0], positionGPS[1], positionGPS[2]);
		fprintf(dataGPSmap, "%f,%f,%f\n", positionGPS[1], positionGPS[0], positionGPS[2]);
		countGPS = countGPS + rateGPS;
	}

	// printf("%d\t", count); // untuk print counter frekuensi
	count++; // untuk count frekuensi

	return SBG_NO_ERROR;
}

SbgErrorCode onLogReceived1(SbgEComHandle* pHandle, SbgEComClass msgClass, SbgEComMsgId msg, const SbgBinaryLogData* pLogData, void* pUserArg)
{
	//
	// Handle separately each received data according to the log ID
	//
	switch (msg)
	{
	case SBG_ECOM_LOG_EKF_NAV:
		// SBG_ECOM_LOG_IMU_DATA
		// SBG_ECOM_LOG_FAST_IMU_DATA
		// SBG_ECOM_LOG_IMU_SHORT
		// SBG_ECOM_LOG_EKF_EULER
		// SBG_ECOM_LOG_EKF_QUAT
		// SBG_ECOM_LOG_EKF_NAV
		// SBG_ECOM_LOG_GPS1_VEL
		// SBG_ECOM_LOG_GPS1_POS

		// printf("%d\t", count); // untuk print counter frekuensi

		printf("Position: %3.1f\t%3.1f\t%3.1f\t Accuracy: %3.1f\t%3.1f\t%3.1f \r",
			pLogData->ekfNavData.position[0], pLogData->ekfNavData.position[1], pLogData->ekfNavData.position[2],
			pLogData->ekfNavData.positionStdDev[0], pLogData->ekfNavData.positionStdDev[1], pLogData->ekfNavData.positionStdDev[2]);

		/*
		printf("Velocity: %3.1f\t%3.1f\t%3.1f\t Accuracy: %3.1f\t%3.1f\t%3.1f \r",
			pLogData->ekfNavData.velocity[0], pLogData->ekfNavData.velocity[1], pLogData->ekfNavData.velocity[2],
			pLogData->ekfNavData.velocityStdDev[0], pLogData->ekfNavData.velocityStdDev[1], pLogData->ekfNavData.velocityStdDev[2]);

		printf("Position: %3.1f\t%3.1f\t%3.1f\t Accuracy: %3.1f\t%3.1f\t%3.1f \r",
			pLogData->ekfNavData.position[0], pLogData->ekfNavData.position[1], pLogData->ekfNavData.position[2],
			pLogData->ekfNavData.positionStdDev[0], pLogData->ekfNavData.positionStdDev[1], pLogData->ekfNavData.positionStdDev[2]);

		printf("GPS Position: %3.1f\t%3.1f\t%3.1f\t Accuracy: %3.1f\t%3.1f\t%3.1f \r",
			pLogData->gpsPosData.latitude, pLogData->gpsPosData.longitude, pLogData->gpsPosData.altitude,
			pLogData->gpsPosData.latitudeAccuracy, pLogData->gpsPosData.longitudeAccuracy, pLogData->gpsPosData.altitudeAccuracy);

		printf("GPS Vel: %3.1f\t%3.1f\t%3.1f\t Accuracy: %3.1f\t%3.1f\t%3.1f \r",
			pLogData->gpsVelData.velocity[0], pLogData->gpsVelData.velocity[1], pLogData->gpsVelData.velocity[2],
			pLogData->gpsVelData.velocityAcc[0], pLogData->gpsVelData.velocityAcc[1], pLogData->gpsVelData.velocityAcc[2]);

		printf("Euler Angles: %3.1f\t%3.1f\t%3.1f\tStd Dev:%3.1f\t%3.1f\t%3.1f \r",
			sbgRadToDegF(pLogData->ekfEulerData.euler[0]), sbgRadToDegF(pLogData->ekfEulerData.euler[1]), sbgRadToDegF(pLogData->ekfEulerData.euler[2]),
			sbgRadToDegF(pLogData->ekfEulerData.eulerStdDev[0]), sbgRadToDegF(pLogData->ekfEulerData.eulerStdDev[1]), sbgRadToDegF(pLogData->ekfEulerData.eulerStdDev[2]));

		printf("Accel: %3.1f\t%3.1f\t%3.1f \r",
			pLogData->imuData.accelerometers[0], pLogData->imuData.accelerometers[1], pLogData->imuData.accelerometers[2]);


		*/
		break;
	default:
		break;
	}

	//count++; // untuk count frekuensi

	return SBG_NO_ERROR;
}

//----------------------------------------------------------------------//
//  Main program                                                        //
//----------------------------------------------------------------------//

/*!
 *	Main entry point.
 *	\param[in]	argc		Number of input arguments.
 *	\param[in]	argv		Input arguments as an array of strings.
 *	\return					0 if no error and -1 in case of error.
 */
int main(int argc, char** argv)
{
	SbgEComHandle			comHandle;
	SbgErrorCode			errorCode;
	SbgInterface			sbgInterface;
	int32					retValue = 0;
	SbgEComDeviceInfo		deviceInfo;

	//
	// Create an interface: 
	// We can choose either a serial for real time operation, or file for previously logged data parsing
	// Note interface closing is also differentiated !
	//
	//errorCode = sbgInterfaceSerialCreate(&sbgInterface, "/dev/cu.usbserial-FTX2GE57", 921600);		// Example for Unix using a FTDI Usb2Uart converter
	errorCode = sbgInterfaceSerialCreate(&sbgInterface, "COM5", 115200);							// Example for Windows serial communication

	//
	// Test that the interface has been created
	//
	if (errorCode == SBG_NO_ERROR)
	{
		// set file to write
		dataINS = _fsopen("C:/Users/Ardi/Documents/dataINS.csv", "w+", _SH_DENYNO);
		dataIMU = _fsopen("C:/Users/Ardi/Documents/dataIMU.csv", "w+", _SH_DENYNO);
		dataGPS = _fsopen("C:/Users/Ardi/Documents/dataGPS.csv", "w+", _SH_DENYNO);
		dataIMUmap = _fsopen("C:/Users/Ardi/Documents/dataIMUmap.txt", "w+", _SH_DENYNO);
		dataGPSmap = _fsopen("C:/Users/Ardi/Documents/dataGPSmap.txt", "w+", _SH_DENYNO);
		setvbuf(dataINS, NULL, _IONBF, 0);
		setvbuf(dataIMU, NULL, _IONBF, 0);
		setvbuf(dataGPS, NULL, _IONBF, 0);
		setvbuf(dataIMUmap, NULL, _IONBF, 0);
		setvbuf(dataGPSmap, NULL, _IONBF, 0);
		fprintf(dataINS, "count,xx,yy,zz\n");
		fprintf(dataIMU, "count,time,xx,yy,zz\n");
		fprintf(dataGPS, "count,time,xx,yy,zz\n");
		fprintf(dataIMUmap, "count,xx,yy,zz\n");
		fprintf(dataGPSmap, "count,xx,yy,zz\n");

		// Create the sbgECom library and associate it with the created interfaces
		//
		errorCode = sbgEComInit(&comHandle, &sbgInterface);

		//
		// Test that the sbgECom has been initialized
		//
		if (errorCode == SBG_NO_ERROR)
		{
			//
			// Get device inforamtions
			//
			errorCode = sbgEComCmdGetInfo(&comHandle, &deviceInfo);

			//
			// Display device information if no error
			//
			if (errorCode == SBG_NO_ERROR)
			{
				printf("Device : %0.9u found\n", deviceInfo.serialNumber);
				printf("Calibration revision %u\n", deviceInfo.calibationRev);
				//printf("Calibration date : %u/%u/%u\n", deviceInfo.calibrationDay, deviceInfo.calibrationMonth, deviceInfo.calibrationYear);
			}
			else
			{
				fprintf(stderr, "ellipseMinimal: Unable to get device information.\n");
			}

			//
			// Configure some output logs to x Hz
			//
			/*
			if (sbgEComCmdOutputSetConf(&comHandle, SBG_ECOM_OUTPUT_PORT_A, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_IMU_DATA, SBG_ECOM_OUTPUT_MODE_MAIN_LOOP) != SBG_NO_ERROR)
			{
				fprintf(stderr, "ellipseMinimal: Unable to configure output log SBG_ECOM_LOG_IMU_DATA.\n");
			}
			if (sbgEComCmdOutputSetConf(&comHandle, SBG_ECOM_OUTPUT_PORT_A, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_EKF_EULER, SBG_ECOM_OUTPUT_MODE_DIV_4) != SBG_NO_ERROR)
			{
				fprintf(stderr, "ellipseMinimal: Unable to configure output log SBG_ECOM_LOG_EKF_EULER.\n");
			}
			if (sbgEComCmdOutputSetConf(&comHandle, SBG_ECOM_OUTPUT_PORT_A, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_EKF_NAV, SBG_ECOM_OUTPUT_MODE_DIV_4) != SBG_NO_ERROR)
			{
				fprintf(stderr, "ellipseMinimal: Unable to configure output log SBG_ECOM_LOG_EKF_NAV.\n");
			}
			*/
			//
			// Display a message for real time data display
			//
			printf("sbgECom properly Initialized.\n");
			printf("sbgECom version %s\n\n", SBG_E_COM_VERSION_STR);
			printf("Display with estimated standard deviation.\n");

			//
			// Define callbacks for received data
			//
			sbgEComSetReceiveLogCallback(&comHandle, onLogReceived, NULL);

			//
			// Loop until the user exist
			//
			while (1)
			{
				//
				// Try to read a frame
				//
				errorCode = sbgEComHandle(&comHandle);

				//
				// Test if we have to release some CPU (no frame received)
				//
				if (errorCode == SBG_NOT_READY)
				{
					//
					// Release CPU
					//
					sbgSleep(1); // ubah untuk atur delay, default 1
				}
				else
				{
					fprintf(stderr, "Error\n");
				}
			}

			//
			// Close the sbgEcom library
			//
			sbgEComClose(&comHandle);
		}
		else
		{
			//
			// Unable to initialize the sbgECom
			//
			fprintf(stderr, "ellipseMinimal: Unable to initialize the sbgECom library.\n");
			retValue = -1;
		}
		
		//
		// Close the interface
		//
		sbgInterfaceSerialDestroy(&sbgInterface);		
	}
	else
	{
		//
		// Unable to create the interface
		//
		fprintf(stderr, "ellipseMinimal: Unable to create the interface.\n");
		retValue = -1;
	}

	//
	// Returns -1 if we have an error
	//
	return retValue;
}
