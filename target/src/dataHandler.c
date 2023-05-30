
/*
 * dataHandler.c
 *
 * Created: 5/1/2023 9:42:59 PM
 *  Author: Marius
 */ 

#include <stdio.h>
#include <stdint.h>
#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>

#include "../Headers/dataHandler.h"

static int16_t dataHandlerTemperature;
static int16_t dataHandlerHumidity;
static int16_t dataHandlerAvgTemperature;
static uint16_t dataHandlerCo2;

static struct ServoLimits servoLimits;
SemaphoreHandle_t xLimitsSemaphorMutex;



int16_t dataHandler_getTempData()
{
	return dataHandlerTemperature;
}


void dataHandler_setTemperature(int16_t sensorTemperature)
{
	dataHandlerTemperature = sensorTemperature;
}


void dataHandler_setAvgTemperature(int16_t sensorAvgTemperature)
{
	dataHandlerAvgTemperature = sensorAvgTemperature;
}


int16_t dataHandler_getAvgTempeature()
{
	return dataHandlerAvgTemperature;
}


void dataHandler_setHumidity(int16_t sensorHumidity)
{
	dataHandlerHumidity = sensorHumidity;
}


int16_t dataHandler_getHumData()
{
	return dataHandlerHumidity;
}


void dataHandler_setCo2(uint16_t sensorCo2)
{
	dataHandlerCo2 = sensorCo2;
}


uint16_t dataHandler_getCo2Data()
{
	return dataHandlerCo2;
}

struct ServoLimits dataHandler_getLimits(){
	if (xLimitsSemaphorMutex != NULL)
	{
		BaseType_t isFree = xSemaphoreTake(xLimitsSemaphorMutex, (TickType_t)10);
		
		if (isFree == pdTRUE)
		{
			xSemaphoreGive(xLimitsSemaphorMutex);
		}
	}
	
	return servoLimits;
}

void dataHandler_setTempLimits(int16_t minTempLimit, int16_t maxTempLimit){
	
	if (xLimitsSemaphorMutex != NULL)
	{
		BaseType_t isFree = xSemaphoreTake(xLimitsSemaphorMutex, (TickType_t)10);
		
		if (isFree == pdTRUE)
		{
			servoLimits.minTempLimit = minTempLimit;
			servoLimits.maxTempLimit = maxTempLimit;
			xSemaphoreGive(xLimitsSemaphorMutex);
		}
	}
}

void dataHandler_createSemaphoreAndMutexes(){
	if (xLimitsSemaphorMutex == NULL)
	{
		xLimitsSemaphorMutex = xSemaphoreCreateMutex();
		
		if (xLimitsSemaphorMutex != NULL)
		{
			xSemaphoreGive(xLimitsSemaphorMutex);
		}
	}
		
}


