cmake_minimum_required(VERSION 2.8.12)
project("CG_Praktikum_1")
message(STATUS ${CMAKE_BUILD_TYPE})

if(MINGW OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11")
endif(MINGW OR CMAKE_COMPILER_IS_GNUCXX)


find_package(Qt5Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


set(sources main.cpp mainwindow.h mainwindow.cpp openglwindow.h openglwindow.cpp)
include(ControlWidget/CMakeLists.txt)

qt5_wrap_ui(ui_h ${ui})

add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
