cmake_minimum_required(VERSION 3.0)
project("CG_Praktikum_4")
message(STATUS ${CMAKE_BUILD_TYPE})

if(MINGW OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11")
endif(MINGW OR CMAKE_COMPILER_IS_GNUCXX)


find_package(Qt5Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(resources "${CMAKE_CURRENT_SOURCE_DIR}/Resources/")
else(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(resources "${CMAKE_CURRENT_BINARY_DIR}/Resources/")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in" "${CMAKE_CURRENT_BINARY_DIR}/config.hpp")


set(sources main.cpp mainwindow.h mainwindow.cpp openglwindow.h openglwindow.cpp)
include(ControlWidget/CMakeLists.txt)
include(Sunsystem/CMakeLists.txt)
include(Resources/CMakeLists.txt)

add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
