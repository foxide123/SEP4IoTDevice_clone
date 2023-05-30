#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <rc_servo.h>

#include <servo.h>
#include <dataHandler.h>


static int8_t position;
static uint8_t servoNo = 0;

static TickType_t xLastWakeTime;
static TickType_t xFrequency;


struct ServoLimits limits;
int16_t minTemp, maxTemp;

void servo_create(){

	rc_servo_initialise();

	printf("Initialization of servo was successful!\n");
}
void servo_turnOn()
{
	int16_t minTemp, maxTemp;
	
	position = 100;
	rc_servo_setPosition(servoNo, position);
	printf("Servo is turned on!\n");
	
	/*
	struct ServoLimits limits = dataHandler_getLimits();
	int16_t minTempLimit = limits.minTempLimit;
	int16_t maxTempLimit = limits.maxTempLimit;
	
	printf("Limits: %d %d\n", minTempLimit, maxTempLimit);
	
	For testing purpose, everything works
	*/
}
void servo_turnOff()
{
	position = 0;
	rc_servo_setPosition(servoNo, position);
	printf("Servo is turned off!\n");
}



limitResult servo_compareTemperatureData(int16_t temperature, int16_t minLimit, int16_t maxLimit)
{
	limitResult result;
	if (temperature >= minLimit && temperature <= maxLimit)
	{
		result = BETWEEN;
	}
	else if (temperature < minLimit)
	{
		result = BELLOW;
	}
	else
	{
		result = ABOVE;
	}
	return result;
}


void servoHandler_Init()
{
	xLastWakeTime  = xTaskGetTickCount();
	xFrequency = pdMS_TO_TICKS(150000); // = 2.5 min
}


void servoHandler_run(void)
{
	printf("Servo Task Started\n");
	xTaskDelayUntil(&xLastWakeTime, xFrequency);
	
    int16_t tempData = dataHandler_getTempData();
	struct ServoLimits limits = dataHandler_getLimits();
	int16_t minTempLimit = limits.minTempLimit;
	int16_t maxTempLimit = limits.maxTempLimit;
	
	limitResult result = servo_compareTemperatureData(tempData, minTempLimit, maxTempLimit);

	if(result == BELLOW)
	{
		servo_turnOff();
	}
	else if(result == ABOVE)
	{
		servo_turnOn();
	}
	else if(result == BETWEEN)
	{
		printf("Terrarium temperature stable");
	}

}

void servo_createTask(void* pvParameters)
{
	(void)pvParameters;
	
	servoHandler_Init();

	for(;;)
	{
		servoHandler_run();
	}
}

