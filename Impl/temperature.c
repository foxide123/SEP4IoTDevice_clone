
/*
 * temperature.c
 *
 * Responsible for the Temperature Sensor HIH8120
 *
 * Created: 5/1/2023 9:51:46 PM
 *  Author: Marius
 */ 

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <ATMEGA_FreeRTOS.h>
#include <task.h>

#include "../Headers/temperature.h"
#include "hih8120.h"

//Buffer for storing sensor data for temperature_getAvgTemperature(); method
#define BUFFER_SIZE 10

int16_t readings[BUFFER_SIZE];


void temperature_create(){
	hih8120_driverReturnCode_t result = hih8120_initialise();
	if(result != HIH8120_OK){
		printf("Initialization of hih8120 failed!\n");
	}
	else{
		printf("Initialization of hih8120 was successful!\n");
	}
}

void temperature_wakeup(){
	hih8120_driverReturnCode_t result = hih8120_wakeup();
	
	if (result != HIH8120_OK)
	{
		printf("Wake up of hih8120 failed!\n");
	}
	else{
		printf("Wake up of hih8120 was successful!\n");
	}
}

void temperature_measure(){
	hih8120_driverReturnCode_t result = hih8120_measure();
	
	if (result != HIH8120_OK)
	{
		printf("Measure of hih8120 failed!\n");
	}
	else{
		printf("Measure of hih8120 was successful!\n");
	}
}

int16_t temperature_getLatestTemperature(){
	
	int16_t measuredTemperature = hih8120_getTemperature_x10();
	printf("Latest recorded temp: %d\n", measuredTemperature);
	return measuredTemperature;
}
int16_t temperature_getLatestTemperatureWithoutPrint(){
	
	int16_t measuredTemperature = hih8120_getTemperature_x10();
	return measuredTemperature;
}

int16_t humidity_getLatestHumidity()
{
	int16_t measureHumidity = hih8120_getHumidityPercent_x10();
	printf("Latest recorded humidity: %d\n", measureHumidity);
	return measureHumidity;
}


void store_data_in_buffer(int16_t reading)
{
	static int buffer_index = 0;
	if (buffer_index < BUFFER_SIZE)
	{
		readings[buffer_index] = reading;
		buffer_index++;
	}else{
		//Buffer is full, overwrite oldest reading
		for (int i = 1; i < BUFFER_SIZE; i++)
		{
			readings[i - 1] = readings[i];
		}
		readings[BUFFER_SIZE - 1] = reading;
	}
}

int16_t temperature_getAvgTemperature()
{
	int16_t sum = 0;
	int count = 0;
	
	
	for (int i = 0; i< BUFFER_SIZE; i++)
	{
		if (readings[i] != 0)
		{
			sum+= readings[i];
			count++;
		}
	}
	
	if (count == 0)
	{
		return 0;
	}
	
	int16_t average = sum/count;
	
	printf("Latest Average Temp read: %d\n", average);
	return average;
}

int16_t get_minimum_value(int16_t readings[BUFFER_SIZE])
{
	int16_t min_value = readings[0];
	for (int i = 1; i < BUFFER_SIZE; i++)
	{
		if (readings[i] < min_value)
		{
			min_value = readings[i];
		}
	}
	return min_value;
}

int16_t get_maximum_value(int16_t readings[BUFFER_SIZE])
{
	int16_t max_value = readings[0];
	for (int i = 1; i < BUFFER_SIZE; i++)
	{
		if (readings[i] > max_value)
		{
			max_value = readings[i];
		}
	}
	return max_value;
}




void temperature_task(void* pvParameters){
	// Remove compiler warnings
	(void)pvParameters;
	
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t xFrequency1 = 1/portTICK_PERIOD_MS; // 1 ms
	const TickType_t xFrequency2 = 50/portTICK_PERIOD_MS; // 50 ms
	const TickType_t xFrequency3 = 30000/portTICK_PERIOD_MS; // 30000 ms

	for (;;)
	{
		printf("Temperature/Humidity Task started\n");
		
		temperature_wakeup();
		vTaskDelay(xFrequency2);
		
		temperature_measure();
		vTaskDelay(xFrequency1);
		
		//Temp average
		int16_t reading = temperature_getLatestTemperatureWithoutPrint();
		store_data_in_buffer(reading);
		
		//wait 30 seconds for next measurement
		xTaskDelayUntil(&xLastWakeTime, xFrequency3);
	}
}