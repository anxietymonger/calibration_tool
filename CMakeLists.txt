project(calibration_tool)
cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_subdirectory("3rdparty/json11")
add_subdirectory("3rdparty/gflags")

link_libraries(json11 gflags ${OpenCV_LIBS})

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(calibration ${SOURCES})
