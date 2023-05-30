#include <gtest/gtest.h>
#include "fff/fff.h"
#include "FreeRTOS_defs/FreeRTOS_FFF_MocksDeclaration.h"

extern "C"
{
#include <sensorHandler.h>
#include <FreeRTOS.h>
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
}

FAKE_VALUE_FUNC(int16_t, temperature_getLatestTemperature);

class SensorHandler_test : public ::testing::Test
{
protected:
  void SetUp() override
  {

    RESET_FAKE(vTaskDelay);
    RESET_FAKE(xTaskCreate);
    RESET_FAKE(xSemaphoreTake);
    RESET_FAKE(xSemaphoreGive);
    RESET_FAKE(xTaskGetTickCount);
    RESET_FAKE(xTaskDelayUntil);
  }
  void TearDown() override
  {
  }
};


TEST_F(SensorHandler_test, Should_properly_initialize_variables_when_sensorHandler_task_init_is_called)
{
  sensorHandler_task_init();

  EXPECT_EQ(xFrequency, 30000/portTICK_PERIOD_MS);
}


TEST_F(SensorHandler_test, Should_call_freertos_methods_when_sensorHandler_task_run_is_called)
{
  TickType_t xLatestWakeTime;
  TickType_t xFrequency = 30000 / portTICK_PERIOD_MS;

  sensorHandler_task_run(&xLatestWakeTime, xFrequency);

  EXPECT_EQ(1, xTaskDelayUntil_fake.call_count);

}
