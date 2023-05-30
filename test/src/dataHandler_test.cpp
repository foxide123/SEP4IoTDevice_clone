#include <gtest/gtest.h>
#include "fff/fff.h"

extern "C"
{
    #include <dataHandler.h>
}

class DataHandler_test : public ::testing::Test
{
protected:
  void SetUp() override
  {
  }
  void TearDown() override
  {
  }
};

TEST_F(DataHandler_test, Should_set_and_return_correct_value_after_calling_dataHandler_getTempData)
{
    //Arrange
    int16_t sensorTemperature = 20;
    
    //Act
    dataHandler_setTemperature(sensorTemperature);
    int16_t result = dataHandler_getTempData();
    EXPECT_EQ(result, sensorTemperature);
}

TEST_F(DataHandler_test, Should_set_and_return_correct_value_after_calling_dataHandler_getAvgTempeature)
{
    //Arrange
    int16_t sensorAvgTemperature = 10;
    
    //Act
    dataHandler_setAvgTemperature(sensorAvgTemperature);
    int16_t result = dataHandler_getAvgTempeature();

    EXPECT_EQ(result, sensorAvgTemperature);
}

TEST_F(DataHandler_test, Should_set_and_return_correct_value_after_calling_dataHandler_setHumidity)
{
    //Arrange
    int16_t sensorHumidity = 30;
    
    //Act
    dataHandler_setHumidity(sensorHumidity);
    int16_t result = dataHandler_getHumData();

    EXPECT_EQ(result, sensorHumidity);
}


TEST_F(DataHandler_test, Should_set_and_return_correct_value_after_calling_dataHandler_getCo2Data)
{
    //Arrange
    int16_t sensorCo2 = 20;
    
    //Act
    dataHandler_setCo2(sensorCo2);
    int16_t result = dataHandler_getCo2Data();

    EXPECT_EQ(result, sensorCo2);
}

TEST_F(DataHandler_test, Should_set_and_return_correct_value_after_calling_dataHandler_getLimits)
{
    int16_t temp_min = 10;
    int16_t temp_max = 20;
    dataHandler_setTempLimits(temp_min, temp_max);

    struct ServoLimits limits = dataHandler_getLimits();
    EXPECT_EQ(limits.minTempLimit, temp_min);
    EXPECT_EQ(limits.maxTempLimit, temp_max);
}
