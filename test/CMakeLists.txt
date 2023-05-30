cmake_minimum_required(VERSION 3.14)
set(projectName appTest)
project(${projectName})

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "--coverage")
set(CMAKE_C_FLAGS "--coverage")

set(DOWNLOAD_EXTRACT_TIMESTAMP true)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest )

set(SOURCES_TEST
    src/co2_test.cpp
    src/temperature_test.cpp
    src/dataHandler_test.cpp
    src/sensorHandler_test.cpp
    src/lora_test.cpp
    src/servo_test.cpp
    src/FreeRTOS_defs/FreeRTOS_FFF_MocksDefinitions.cpp
    ../target/src/co2.c
    ../target/src/temperature.c
    ../target/src/LoRaWANHandler.c
    ../target/src/dataHandler.c
    ../target/src/sensorHandler.c
    ../target/src/servo.c
    ${SRCS}
)




# This makes the test.cpp files able to #include target libraries without errors.  
include_directories(
  include
  src/FreeRTOS_defs  # Mock and FreeRTOS definitions
  ../target/include
  ../target/lib/drivers
  ../target/lib/FreeRTOS
)

# Something to 
enable_testing()

add_executable(
  ${projectName}
  ${SOURCES_TEST}
)
target_link_libraries(
  ${projectName}
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${projectName})