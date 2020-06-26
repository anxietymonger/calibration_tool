project(gion2019plus)
cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

add_subdirectory("3rdparty/json11")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
link_libraries(json11 ${OpenCV_LIBS})
add_executable(calibration main.cpp)
