#include <ATMEGA_FreeRTOS.h>

void lora_handler_task( void *pvParameters );
void lora_handler_initialise(UBaseType_t lora_handler_task_priority);
void _lora_setup();
void lora_handler_task( void *pvParameters );

void lora_handler_task_init();
void lora_handler_run(TickType_t* xLatestWakeTime, TickType_t xFrequency);
