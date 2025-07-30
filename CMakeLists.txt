cmake_minimum_required(VERSION 3.10)
project(XOGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Use vcpkg toolchain if passed in
if(DEFINED CMAKE_TOOLCHAIN_FILE)
  message(STATUS "Using toolchain: ${CMAKE_TOOLCHAIN_FILE}")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)


file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(XOGame ${SOURCES} main.cpp)

target_link_libraries(XOGame PRIVATE
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
)

add_custom_command(TARGET XOGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:XOGame>/assets)