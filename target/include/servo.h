#pragma once
#include <stdlib.h>
#include <stdint.h>

// Using enums for a cleaner approach for the 3 states that we might have
typedef enum {
    ABOVE,
    BETWEEN,
    BELLOW
} limitResult;

void servo_create();
void servo_turnOn();
void servo_turnOff();
void servoHandler_Init();
void servoHandler_run();
void servo_createTask(void* pvParameters);
limitResult servo_compareTemperatureData(int16_t temperature, int16_t minLimit, int16_t maxLimit);
