cmake_minimum_required(VERSION 3.10)

project(modbus-data LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(COMPILE_FLAGS -Wall -Wextra -Werror)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
endif()

set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(HEADERS ${SOURCE_DIR}/modbus_data.hpp)
set(SOURCES ${SOURCE_DIR}/example.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
