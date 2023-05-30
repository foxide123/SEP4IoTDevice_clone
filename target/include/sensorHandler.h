#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <ATMEGA_FreeRTOS.h>
#include <task.h>


void sensorsHandler_createSensors();
void sensorHandler_task_init();
void sensorHandler_task_run(TickType_t* xLastWakeTime, TickType_t xFrequency);
void sensorsHandler_task(void *pvParameters);

extern TickType_t xFrequency;