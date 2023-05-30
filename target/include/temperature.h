/*
 * temperature.h
 *
 * Created: 5/1/2023 9:51:31 PM
 *  Author: Marius
 */ 


#pragma once
#include <stdlib.h>
#include <stdint.h>
#define BUFFER_SIZE 10

void temperature_create();
void temperature_wakeup();
void temperature_measure();
int16_t temperature_getLatestTemperature();
int16_t temperature_getLatestTemperatureWithoutPrint();
int16_t humidity_getLatestHumidity();
void store_data_in_buffer(int16_t reading);
int16_t temperature_getAvgTemperature();
void temperature_task_init();
void temperature_task_run(TickType_t* xLastWakeTime, TickType_t xFrequency1,TickType_t xFrequency2,TickType_t xFrequency3);
void temperature_task(void* pvParameters);
int16_t get_maximum_value(int16_t readings[]);
int16_t get_minimum_value(int16_t readings[]);

//to set buffer for testing
extern int16_t readings[BUFFER_SIZE];
extern TickType_t xLastWakeTime;

