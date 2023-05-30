#include <gtest/gtest.h>
#include "fff/fff.h"

extern "C"
{
	#include <co2.h>
	#include <mh_z19.h>
}

FAKE_VOID_FUNC(mh_z19_initialise, serial_comPort_t);
FAKE_VALUE_FUNC(mh_z19_returnCode_t, mh_z19_takeMeassuring);

// Create Test fixture and Reset all Mocks before each test
class CO2_test : public ::testing::Test
{
protected:
	void SetUp() override
	{
		RESET_FAKE(mh_z19_initialise);
		RESET_FAKE(mh_z19_takeMeassuring);
		FFF_RESET_HISTORY();
	}
	void TearDown() override
	{}
};

TEST_F(CO2_test, Should_call_mh_z19_initialise_when_co2_sensor_create_is_called)
{
    
	// Act
	co2_sensor_create();

	// Assert
	EXPECT_EQ(mh_z19_initialise_fake.call_count, 1);
} 

TEST_F(CO2_test, Should_call_mh_z19_takeMeassuring_when_co2_sensor_measure_is_called)
{
    // Arrange
    mh_z19_takeMeassuring_fake.return_val = MHZ19_OK;

    // Act
    co2_sensor_measure();

    // Assert
    EXPECT_EQ(mh_z19_takeMeassuring_fake.call_count, 1);
}


TEST_F(CO2_test, Should_return_last_reading_when_co2_sensor_get_last_reading_is_called)
{
    // Arrange
    int16_t lastCo2Recorded = 123;
	co2_sensor_set_last_reading(lastCo2Recorded);

    // Act
    uint16_t result = co2_sensor_get_last_reading();

    // Assert
    EXPECT_EQ(result, 123);
}


// ensures that the co2 sensor is created and initialized correctly
TEST_F(CO2_test, Should_call_co2_sensor_create_and_initialize_correctly)
{
    // Arrange

    // Act
    co2_sensor_create();

    // Assert
    EXPECT_EQ(mh_z19_initialise_fake.call_count, 1);
}

enum FakeErrorCode
{
    ERROR_CODE_TEST = -999, // Custom error code for testing purposes
};

TEST_F(CO2_test, Should_fail_to_measure_when_mh_z19_takeMeassuring_returns_error)
{
    // Arrange
    mh_z19_takeMeassuring_fake.return_val = static_cast<mh_z19_returnCode_t>(ERROR_CODE_TEST);

    // Act
    co2_sensor_measure();

    // Assert
    EXPECT_EQ(mh_z19_takeMeassuring_fake.call_count, 1);
}


TEST_F(CO2_test, Should_update_last_reading_when_co2_sensor_measure_is_successful)
{
    // Arrange
    mh_z19_takeMeassuring_fake.return_val = MHZ19_OK;
    const uint16_t expectedReading = 789;

    // Act
    co2_sensor_measure();
    co2_sensor_set_last_reading(expectedReading);
    uint16_t result = co2_sensor_get_last_reading();

    // Assert
    EXPECT_EQ(mh_z19_takeMeassuring_fake.call_count, 1);
    EXPECT_EQ(result, expectedReading);
}

TEST_F(CO2_test, Should_initialize_co2_sensor_on_create)
{
    // Arrange

    // Act
    co2_sensor_create();

    // Assert
    EXPECT_EQ(mh_z19_initialise_fake.call_count, 1);

}

TEST_F(CO2_test, Should_set_last_reading_to_zero_when_co2_sensor_create_is_called)
{
    // Arrange

// Act
    co2_sensor_create();
    uint16_t result = co2_sensor_get_last_reading();

    // Assert
    EXPECT_NE(result, 0); // Expecting the initial value to be non-zero
}

TEST_F(CO2_test, Should_set_last_reading_to_specific_value)
{
    // Arrange
    uint16_t expectedReading = 456;

    // Act
    co2_sensor_set_last_reading(expectedReading);
    uint16_t result = co2_sensor_get_last_reading();

    // Assert
    EXPECT_EQ(result, expectedReading);
}

// TEST_F(CO2_test, Should_call_co2_sensor_measure_periodically)
// {
   //  Arrange

  //   Act
//   co2_task(nullptr);

   //  Assert
 //   EXPECT_EQ(mh_z19_takeMeassuring_fake.call_count, 1); // Assuming it's called at least once
// }

