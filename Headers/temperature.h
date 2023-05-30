
/*
 * temperature.h
 *
 * Created: 5/1/2023 9:51:31 PM
 *  Author: Marius
 */ 


#pragma once
#include <stdlib.h>
#include <stdint.h>

void temperature_create();
int16_t temperature_getLatestTemperature();
int16_t temperature_getAvgTemperature();
int16_t get_maximum_value(int16_t readings[]);
int16_t get_minimum_value(int16_t readings[]);


int16_t humidity_getLatestHumidity();
void temperature_task();
