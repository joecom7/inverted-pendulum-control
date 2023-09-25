cmake_minimum_required(VERSION 2.8.12)
project (inverted-pendulum-control)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(EXECUTABLE_NAME stabilizer)

file(GLOB ${PROJECT_NAME}_SOURCES Constants.cpp src/*/*.cpp src/*/*/*.cpp)

add_subdirectory(meca500_ethercat_cpp)

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

add_executable(${EXECUTABLE_NAME} stabilizer.cpp)
target_include_directories(${EXECUTABLE_NAME} PUBLIC include)
target_link_libraries(${EXECUTABLE_NAME} meca500_driver)
target_link_libraries(${EXECUTABLE_NAME} pigpio)
target_link_libraries(${EXECUTABLE_NAME} ${PROJECT_NAME})

add_executable(test-controllo test/test_controllo.cpp)
target_include_directories(test-controllo PUBLIC include)
target_link_libraries(test-controllo meca500_driver)
target_link_libraries(test-controllo pigpio)
target_link_libraries(test-controllo ${PROJECT_NAME})

add_executable(test-caduta-libera test/test_caduta_libera.cpp)
target_include_directories(test-caduta-libera PUBLIC include)
target_link_libraries(test-caduta-libera meca500_driver)
target_link_libraries(test-caduta-libera pigpio)
target_link_libraries(test-caduta-libera ${PROJECT_NAME})

add_executable(test-feedback-posa test/test_feedback_posa.cpp)
target_include_directories(test-feedback-posa PUBLIC include)
target_link_libraries(test-feedback-posa meca500_driver)
target_link_libraries(test-feedback-posa pigpio)
target_link_libraries(test-feedback-posa ${PROJECT_NAME})

add_executable(test-onda-quadra test/test_onda_quadra.cpp)
target_include_directories(test-onda-quadra PUBLIC include)
target_link_libraries(test-onda-quadra meca500_driver)
target_link_libraries(test-onda-quadra pigpio)
target_link_libraries(test-onda-quadra ${PROJECT_NAME})