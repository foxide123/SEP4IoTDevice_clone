
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>
#include <mh_z19.h>

#include "../Headers/co2.h"

uint16_t lastCO2Recorded;
mh_z19_returnCode_t status;


void co2_callback(uint16_t level)
{
	lastCO2Recorded = level;
}



void co2_sensor_create()
{
	mh_z19_initialise(ser_USART3);
	mh_z19_injectCallBack(co2_callback);

	
}




void co2_sensor_measure()
{
	// Trigger a new CO2 measurement
	mh_z19_returnCode_t status = mh_z19_takeMeassuring();
	if (status != MHZ19_OK)
	{
		printf("Measure of MHZ19 failed!\n");
	}
	else{
		printf("Measure of MHZ19 was successful!\n");
	}
}


uint16_t co2_sensor_get_last_reading()
{
	
	// Return the last CO2 reading
	return lastCO2Recorded;
}


void co2_task(void* pvParameters)
{
	(void) pvParameters;

	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t xFrequency1 = 1/portTICK_PERIOD_MS; // 1 ms
	const TickType_t xFrequency2 = 50/portTICK_PERIOD_MS; // 50 ms
	const TickType_t xFrequency3 = 30000/portTICK_PERIOD_MS; // 30000 ms


	for (;;) {
		printf("CO2 Task started\n");
		co2_sensor_measure();
		xTaskDelayUntil(&xLastWakeTime, xFrequency2);
		
		printf("Last recorded co2 value: %u\n",co2_sensor_get_last_reading());
		xTaskDelayUntil(&xLastWakeTime, xFrequency3);
	}
}