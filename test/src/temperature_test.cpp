#include <gtest/gtest.h>
#include "fff/fff.h"
#include "FreeRTOS_defs/FreeRTOS_FFF_MocksDeclaration.h"

extern "C"
{
#include <temperature.h>
#include <hih8120.h>
#include <FreeRTOS.h>
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#define BUFFER_SIZE 10
  // int16_t readings[BUFFER_SIZE];
}

DEFINE_FFF_GLOBALS;
FAKE_VALUE_FUNC(hih8120_driverReturnCode_t, hih8120_initialise);
FAKE_VALUE_FUNC(hih8120_driverReturnCode_t, hih8120_wakeup);
FAKE_VALUE_FUNC(hih8120_driverReturnCode_t, hih8120_measure);
FAKE_VALUE_FUNC(int16_t, hih8120_getTemperature_x10);
FAKE_VALUE_FUNC(uint16_t, hih8120_getHumidityPercent_x10);

// Create Test fixture and Reset all Mocks before each test
class Temperature_test : public ::testing::Test
{
protected:
  void SetUp() override
  {
    RESET_FAKE(hih8120_initialise);
    RESET_FAKE(hih8120_wakeup);
    RESET_FAKE(hih8120_measure);
    RESET_FAKE(hih8120_getTemperature_x10);
    RESET_FAKE(hih8120_getHumidityPercent_x10);
    FFF_RESET_HISTORY();
  }
  void TearDown() override
  {
  }
};

TEST_F(Temperature_test, Should_call_hih8120_initialise_when_temperature_create_is_called)
{
  // Arrange
  hih8120_initialise_fake.return_val = HIH8120_OK;
  // Act
  temperature_create();
  // Assert
  EXPECT_EQ(hih8120_initialise_fake.call_count, 1);
}

TEST_F(Temperature_test, Should_call_hih8120_wakeup_when_temperature_wakeup_is_called)
{
  // Arrange
  hih8120_wakeup_fake.return_val = HIH8120_OK;
  // Act
  temperature_wakeup();
  // Assert
  EXPECT_EQ(hih8120_wakeup_fake.call_count, 1);
}

TEST_F(Temperature_test, Should_call_hih8120_measure_when_temperature_measure_is_called)
{
  // Arrange
  hih8120_measure_fake.return_val = HIH8120_OK;
  // Act
  temperature_measure();
  // Assert
  EXPECT_EQ(hih8120_measure_fake.call_count, 1);
}

TEST_F(Temperature_test, Should_return_correct_value_when_temperature_getLatestTemperature_is_called)
{
  // Arrange
  hih8120_getTemperature_x10_fake.return_val = 30;
  int16_t _temperature;
  // Act
  _temperature = temperature_getLatestTemperature();
  // Assert
  EXPECT_EQ(hih8120_getTemperature_x10_fake.call_count, 1);
  EXPECT_EQ(_temperature, 30);
}

TEST_F(Temperature_test, Should_return_correct_value_when_humidity_getLatestHumidity_is_called)
{
  // Arrange
  hih8120_getHumidityPercent_x10_fake.return_val = 5;
  uint16_t _humidity;
  // Act
  _humidity = humidity_getLatestHumidity();
  // Assert
  EXPECT_EQ(hih8120_getHumidityPercent_x10_fake.call_count, 1);
  EXPECT_EQ(_humidity, 5);
}

TEST_F(Temperature_test, Should_store_data_in_buffer){
  //clear readings
  memset(readings, 0, sizeof(readings));

  store_data_in_buffer(10);
  store_data_in_buffer(11);
  store_data_in_buffer(12);
  EXPECT_EQ(readings[0], 10);
  EXPECT_EQ(readings[1], 11);
  EXPECT_EQ(readings[2], 12);
}

TEST_F(Temperature_test, Should_overwrite_readings_when_buffer_is_full){
  //For BUFFER_SIZE = 10

  //clear readings
  memset(readings, 0, sizeof(readings));

  store_data_in_buffer(10);
  store_data_in_buffer(11);
  store_data_in_buffer(12);
  
  store_data_in_buffer(13);
  store_data_in_buffer(14);
  store_data_in_buffer(15);

  store_data_in_buffer(16);
  store_data_in_buffer(17);
  store_data_in_buffer(18);

  store_data_in_buffer(19);
  store_data_in_buffer(20);
  EXPECT_EQ(readings[0], 11);
}

TEST_F(Temperature_test, Should_return_correct_value_when_temperature_getAvgTemperature_is_called)
{
  //clear readings
  memset(readings, 0, sizeof(readings));

  store_data_in_buffer(10);
  store_data_in_buffer(20);
  store_data_in_buffer(30);
  
  EXPECT_EQ(temperature_getAvgTemperature(), 20);
}

TEST_F(Temperature_test, Should_return_0_when_readings_buffer_is_empty_when_temperature_getAvgTemperature_is_called)
{
  memset(readings, 0, sizeof(readings));

  EXPECT_EQ(temperature_getAvgTemperature(), 0);
}

TEST_F(Temperature_test, Should_return_min_value_out_of_all_readings_when_get_minimum_value_is_called)
{
  int16_t readings[BUFFER_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int16_t expected_min = 1;

  int16_t result = get_minimum_value(readings);

  EXPECT_EQ(result, expected_min);
}

TEST_F(Temperature_test, Should_return_max_value_out_of_all_readings_when_get_minimum_value_is_called)
{
  int16_t readings[BUFFER_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int16_t expected_max = 10;

  int16_t result = get_maximum_value(readings);

  EXPECT_EQ(result, expected_max);
}

class Temperature_freertos_test : public ::testing::Test
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
    FFF_RESET_HISTORY();
  }
  void TearDown() override
  {
  }
};

TEST_F(Temperature_freertos_test, temperature_task_init_is_called)
{
  temperature_task_init();

  ASSERT_EQ(hih8120_initialise_fake.call_count, 1);
}

TEST_F(Temperature_freertos_test, Should_call_freertos_methods_when_sensorHandler_task_run_is_called){
  TickType_t xLastWakeTime = xTaskGetTickCount();
	TickType_t xFrequency1 = 1/portTICK_PERIOD_MS; // 1 ms
	TickType_t xFrequency2 = 50/portTICK_PERIOD_MS; // 50 ms
	TickType_t xFrequency3 = 30000/portTICK_PERIOD_MS; // 30000 ms

  temperature_task_run(&xLastWakeTime, xFrequency1, xFrequency2, xFrequency3);

  EXPECT_EQ(2, vTaskDelay_fake.call_count);
}
