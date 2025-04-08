cmake_minimum_required(VERSION 3.10)
project(ImageProcessingApp)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} include)

file(GLOB SOURCES "src/*.cpp" "main.cpp")

add_executable(ImageProcessingApp ${SOURCES})
target_link_libraries(ImageProcessingApp ${OpenCV_LIBS})