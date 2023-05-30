#include <gtest/gtest.h>
#include "fff/fff.h"
#include "FreeRTOS_defs/FreeRTOS_FFF_MocksDeclaration.h"

extern "C" {
    #include <servo.h>
    #include <dataHandler.h>
    #include <rc_servo.h>
    #include <FreeRTOS.h>
    #include <ATMEGA_FreeRTOS.h>
    #include <task.h>
}

FAKE_VOID_FUNC(rc_servo_initialise);
FAKE_VOID_FUNC(rc_servo_setPosition, uint8_t, int8_t);

void rc_servo_setPosition_custom_fake(uint8_t servoNo, int8_t position) {
    rc_servo_setPosition_fake.arg0_val = servoNo;
    rc_servo_setPosition_fake.arg1_val = position;
}

class ServoTest : public ::testing::Test {
protected:
    void SetUp() override {
        RESET_FAKE(rc_servo_initialise);
        RESET_FAKE(rc_servo_setPosition);
        FFF_RESET_HISTORY();
    }
};

TEST_F(ServoTest, ShouldCallRcServoInitialiseWhenServoCreateIsCalled) {
    // Act
    servo_create();
    // Assert
    EXPECT_EQ(rc_servo_initialise_fake.call_count, 1);
}

TEST_F(ServoTest, ShouldCallRcServoSetPositionWithCorrectParametersWhenServoTurnOnIsCalled) {
    // Arrange
    uint8_t servoNo = 0;
    int8_t position = 100;

    // Act
    servo_turnOn();

    // Assert
    EXPECT_EQ(rc_servo_setPosition_fake.call_count, 1);
    EXPECT_EQ(rc_servo_setPosition_fake.arg0_val, servoNo);
    EXPECT_EQ(rc_servo_setPosition_fake.arg1_val, position);
}

TEST_F(ServoTest, ShouldCallRcServoSetPositionWithCorrectParametersWhenServoTurnOffIsCalled) {
    // Arrange
    uint8_t servoNo = 0;
    int8_t position = 0;

    // Act
    servo_turnOff();

    // Assert
    EXPECT_EQ(rc_servo_setPosition_fake.call_count, 1);
    EXPECT_EQ(rc_servo_setPosition_fake.arg0_val, servoNo);
    EXPECT_EQ(rc_servo_setPosition_fake.arg1_val, position);
}


TEST_F(ServoTest, ShouldCallServoTurnOffWhenTemperatureBelowMinLimit) {
    // Arrange
    int16_t temperature = 10;
    int16_t minLimit = 20;
    int16_t maxLimit = 30;
    //rc_servo_setPosition_fake.custom_fake = rc_servo_setPosition_custom_fake;
    //rc_servo_setPosition_fake.arg0_val = 0;
    //rc_servo_setPosition_fake.arg1_val = 30;

    // Act
    dataHandler_setTempLimits(20,30);
    servoHandler_Init();
    servoHandler_run();
    servo_compareTemperatureData(temperature, minLimit, maxLimit);

    // Assert
    EXPECT_EQ(rc_servo_setPosition_fake.call_count, 1);
}

TEST_F(ServoTest, ShouldCallServoTurnOnWhenTemperatureAboveMaxLimit) {
    // Arrange
    int16_t temperature = 40;
    int16_t minLimit = 20;
    int16_t maxLimit = 30;

    // Act
    dataHandler_setTempLimits(20,30);
    servoHandler_Init();
    servo_compareTemperatureData(temperature, minLimit, maxLimit);
    servoHandler_run();

    // Assert
    EXPECT_EQ(rc_servo_setPosition_fake.call_count, 1);
}


TEST_F(ServoTest, ShouldNotCallServoTurnOnOrServoTurnOffWhenTemperatureWithinLimits) {
    // Arrange
    int16_t temperature = 25;
    int16_t minLimit = 20;
    int16_t maxLimit = 30;
    rc_servo_setPosition_fake.custom_fake = rc_servo_setPosition_custom_fake;

    // Act
    servo_compareTemperatureData(temperature, minLimit, maxLimit);

    // Assert
    EXPECT_EQ(rc_servo_setPosition_fake.call_count, 0);
}
