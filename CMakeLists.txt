cmake_minimum_required(VERSION 3.10)
project(MyGame)

set(CMAKE_CXX_STANDARD 17)

# SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Project structure
include_directories(include)
file(GLOB SOURCES "src/*.cpp" "main.cpp")

add_executable(MyGame ${SOURCES})
target_link_libraries(MyGame ${SDL2_LIBRARIES})