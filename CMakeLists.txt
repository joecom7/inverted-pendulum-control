cmake_minimum_required(VERSION 2.8.12)
project (inverted-pendulum-control)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(EXECUTABLE_NAME stabilizer)

file(GLOB ${PROJECT_NAME}_SOURCES Constants.cpp src/*/*.cpp src/*/*/*.cpp)

add_subdirectory(meca500_ethercat_cpp)

add_executable(${EXECUTABLE_NAME} stabilizer.cpp ${${PROJECT_NAME}_SOURCES})
add_executable(test-feedback test_feedback.cpp ${${PROJECT_NAME}_SOURCES})

target_include_directories(${EXECUTABLE_NAME} PUBLIC include)
target_include_directories(test-feedback PUBLIC include)

target_link_libraries(${EXECUTABLE_NAME} meca500_driver)
target_link_libraries(${EXECUTABLE_NAME} pigpio)

target_link_libraries(test-feedback meca500_driver)
target_link_libraries(test-feedback pigpio)