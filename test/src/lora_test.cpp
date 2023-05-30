#include <gtest/gtest.h>
#include "fff/fff.h"
#include "FreeRTOS_defs/FreeRTOS_FFF_MocksDeclaration.h"

extern "C"
{
#include <lora_driver.h>
#include <status_leds.h>
#include <LoRaWANHandler.h>
#include <dataHandler.h>
}

FAKE_VOID_FUNC(lora_driver_resetRn2483, uint8_t);
FAKE_VOID_FUNC(lora_driver_flushBuffers);
FAKE_VOID_FUNC(status_leds_slowBlink, status_leds_t);
FAKE_VOID_FUNC(status_leds_longPuls, status_leds_t);
FAKE_VOID_FUNC(status_leds_ledOn, status_leds_t);
FAKE_VOID_FUNC(status_leds_ledOff, status_leds_t);
FAKE_VOID_FUNC(status_leds_fastBlink, status_leds_t);
FAKE_VOID_FUNC(status_leds_shortPuls, status_leds_t);

FAKE_VALUE_FUNC(char *, lora_driver_mapReturnCodeToText, lora_driver_returnCode_t);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_join, lora_driver_joinMode_t);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_sendUploadMessage, bool, lora_driver_payload_t *);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_getRn2483Hweui, char *);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_setOtaaIdentity, char *, char *, char *);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_setDeviceIdentifier, const char *);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_configureToEu868);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_setAdaptiveDataRate, lora_driver_adaptiveDataRate_t);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_setReceiveDelay, uint16_t);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_rn2483FactoryReset);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_saveMac);

class Lora_test : public ::testing::Test
{
protected:
  void SetUp() override
  {
    RESET_FAKE(lora_driver_mapReturnCodeToText);
    RESET_FAKE(lora_driver_getRn2483Hweui);
    RESET_FAKE(lora_driver_join);
    RESET_FAKE(lora_driver_resetRn2483);
    RESET_FAKE(lora_driver_flushBuffers);
    RESET_FAKE(lora_driver_sendUploadMessage);
    RESET_FAKE(lora_driver_setOtaaIdentity);
    RESET_FAKE(lora_driver_setDeviceIdentifier);
    RESET_FAKE(lora_driver_setAdaptiveDataRate);
    RESET_FAKE(lora_driver_setReceiveDelay);
    RESET_FAKE(lora_driver_rn2483FactoryReset);
    RESET_FAKE(lora_driver_saveMac);

    RESET_FAKE(status_leds_slowBlink);
    RESET_FAKE(status_leds_longPuls);
    RESET_FAKE(status_leds_ledOn);
    RESET_FAKE(status_leds_ledOff);
    RESET_FAKE(status_leds_fastBlink);
    RESET_FAKE(status_leds_shortPuls);
    RESET_FAKE(xTaskCreate);
    RESET_FAKE(vTaskDelay);
    RESET_FAKE(xTaskGetTickCount);
    RESET_FAKE(xTaskDelayUntil);
    RESET_FAKE(taskYIELD);
    FFF_RESET_HISTORY();
  }
  void TearDown() override
  {
  }
};

// initialise
TEST_F(Lora_test, Should_call_xTaskCreate_with_correct_parameter_when_lora_handler_initialise_is_called)
{
  UBaseType_t priority = 2;

  lora_handler_initialise(priority);

  EXPECT_EQ(xTaskCreate_fake.call_count, 1);
}

TEST_F(Lora_test, Should_call_lora_driver_sendUploadMessage_when_lora_handler_run_is_called)
{
  TickType_t xWakeTime;
  TickType_t xFrequency = 30000 / portTICK_PERIOD_MS;
  lora_driver_join_fake.return_val = LORA_ACCEPTED;
  lora_driver_sendUploadMessage_fake.return_val = LORA_ACCEPTED;
  lora_handler_run(&xWakeTime , xFrequency);
  EXPECT_EQ(lora_driver_sendUploadMessage_fake.call_count, 1);
}

TEST_F(Lora_test, Should_send_two_messages_when_lora_handler_task_is_called_twice)
{
  TickType_t xWakeTime;
  TickType_t xFrequency = 30000 / portTICK_PERIOD_MS;
  lora_driver_join_fake.return_val = LORA_ACCEPTED;
  lora_driver_sendUploadMessage_fake.return_val = LORA_ACCEPTED;

  for(int i=0; i<2; i++)
  {
    lora_handler_run(&xWakeTime , xFrequency);
  }

  EXPECT_EQ(lora_driver_sendUploadMessage_fake.call_count, 2);
}


TEST_F(Lora_test, Should_call_lora_methods_when_lora_setup_is_called_with_LORA_ACCEPTED)
{
  lora_driver_join_fake.return_val = LORA_ACCEPTED;
  lora_driver_mapReturnCodeToText_fake.return_val = "mapReturnCodeToTextString";
  _lora_setup();

  EXPECT_EQ(status_leds_slowBlink_fake.call_count, 1);
  // EXPECT_EQ(lora_driver_mapReturnCodeToText_fake.call_count, 9);
  EXPECT_EQ(lora_driver_rn2483FactoryReset_fake.call_count, 1);
  EXPECT_EQ(lora_driver_configureToEu868_fake.call_count, 1);
  EXPECT_EQ(lora_driver_getRn2483Hweui_fake.call_count, 1);
  EXPECT_EQ(lora_driver_setDeviceIdentifier_fake.call_count, 1);
  EXPECT_EQ(lora_driver_setOtaaIdentity_fake.call_count, 1);
  EXPECT_EQ(lora_driver_saveMac_fake.call_count, 1);
  EXPECT_EQ(lora_driver_setAdaptiveDataRate_fake.call_count, 1);
  EXPECT_EQ(lora_driver_setReceiveDelay_fake.call_count, 1);
  EXPECT_EQ(lora_driver_join_fake.call_count, 1);
  //EXPECT_EQ(status_leds_longPuls_fake.call_count, 1);
}
/*
// handler_task
TEST_F(Lora_test, Should_call_methods_when_lora_handler_task_is_called)
{
  lora_handler_task(nullptr);
  EXPECT_EQ(lora_driver_resetRn2483_fake.call_count, 2);
  EXPECT_EQ(vTaskDelay_fake.call_count, 2);
  EXPECT_EQ(lora_driver_flushBuffers_fake.call_count, 1);
  EXPECT_EQ(lora_driver_mapReturnCodeToText_fake.call_count, 1);
  EXPECT_EQ(lora_driver_sendUploadMessage_fake.call_count, 1);
}

TEST_F(Lora_test, Should_join_LoRaWAN_during_second_try_when_lora_setup_is_called)
{
  lora_driver_join_fake.return_val = LORA_ERROR;
  lora_driver_join_fake.return_val_history[1] = LORA_ACCEPTED;
  lora_driver_mapReturnCodeToText_fake.return_val = "mapReturnCodeToTextString";

  _lora_setup();
  EXPECT_EQ(lora_driver_join_fake.call_count, 2);
  EXPECT_EQ(vTaskDelay_fake.call_count, 1);
}
*/