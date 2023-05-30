#include <stdio.h>
#include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <stdio_driver.h>
#include <serial.h>

// Needed for LoRaWAN
#include <lora_driver.h>
#include <status_leds.h>

#include <display_7seg.h>


#include "Headers/dataHandler.h"
#include "Headers/sensorHandler.h"
#include "Headers/temperature.h"
#include "Headers/servo.h"

// define two Tasks
void task1( void *pvParameters );
void task2( void *pvParameters );

// define semaphore handle
SemaphoreHandle_t xTestSemaphore;

MessageBufferHandle_t downLinkMessageBufferHandle;

// Prototype for LoRaWAN handler
void lora_handler_initialise(UBaseType_t lora_handler_task_priority);

/*-----------------------------------------------------------*/
void create_tasks_and_semaphores(void)
{
	dataHandler_createSemaphoreAndMutexes();
		
	xTaskCreate(
	sensorsHandler_task
	,  "sensorHandlerTask"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );


	xTaskCreate(
	servo_createTask
	,  "servoHandlerTask"
	,  configMINIMAL_STACK_SIZE
	,  NULL
	,  2
	,  NULL );
	
}


/*-----------------------------------------------------------*/
void initialiseSystem()
{
	// Set output ports for leds used in the example
	DDRA |= _BV(DDA0) | _BV(DDA7);

	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdio_initialise(ser_USART0);
	
	//Create the temp sensor through sensorHandler
	sensorsHandler_createSensors();
	
	//Create servo
	servo_create();
	
	// Let's create some tasks
	create_tasks_and_semaphores();
	
	//Some default values for the Limits.
	dataHandler_setTempLimits(240,260);
	
	// Here the call back function is not needed
	display_7seg_initialise(NULL);
	
	// Power up the display
	display_7seg_powerUp();
	
	display_7seg_displayHex("B00B5");


	// ****************** BELOW IS LoRaWAN initialisation **************************
	// Status Leds driver
	status_leds_initialise(5); // Priority 5 for internal task
	
	//Initialise the LoRaWAN driver with downlink
	downLinkMessageBufferHandle = xMessageBufferCreate(sizeof(lora_driver_payload_t)*2); // Here I make room for two downlink messages in the message buffer
	lora_driver_initialise(ser_USART1, downLinkMessageBufferHandle); // The parameter is the USART port the RN2483 module is connected to - in this case USART1 - here no message buffer for down-link messages are defined

	
	// Create LoRaWAN task and start it up with priority 3
	lora_handler_initialise(3);
}

/*-----------------------------------------------------------*/
int main(void)
{
	initialiseSystem(); // Must be done as the very first thing!!
	printf("Program Started!!\n");
	vTaskStartScheduler(); // Initialise and run the freeRTOS scheduler. Execution should never return from here.

	/* Replace with your application code */
	while (1)
	{
	}
}

