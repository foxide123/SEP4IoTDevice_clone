
/*
 * servo.h
 *
 * Created: 5/21/2023 9:07:19 PM
 *  Author: Marius
 */ 

#pragma once
#include <stdlib.h>
#include <stdint.h>

//Using enums for a cleaner approach for the 3 states that we might have
typedef enum{
	ABOVE,
	BETWEEN,
	BELLOW
}limitResult;

void servo_create();
void servo_turnOn();
void servo_turnOff();

void servoHandler_Init();
void servoHandler_run();
void servo_createTask(void* pvParameters);
void servo_task();