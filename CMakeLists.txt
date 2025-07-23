cmake_minimum_required(VERSION 3.10)
project(MyGame)

set(CMAKE_CXX_STANDARD 17)

# SDL2 SDL2_image
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Manually locate SDL_image (works well on macOS/Homebrew)
include_directories(/opt/homebrew/include/SDL2)  # <-- adjust if you're on Intel Mac
link_directories(/opt/homebrew/lib) 

# Project structure
include_directories(include)
file(GLOB SOURCES "src/*.cpp" "main.cpp")

add_executable(MyGame ${SOURCES})
target_link_libraries(MyGame ${SDL2_LIBRARIES} SDL2_image)

add_custom_command(TARGET MyGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:MyGame>/assets)