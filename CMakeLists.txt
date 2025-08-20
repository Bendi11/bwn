cmake_minimum_required(VERSION 3.20)

project(
    bwn
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_REQUIRE_CXX_STANDARD ON)

find_library(
    pa
    pulse
    REQUIRED
)

file(
    GLOB_RECURSE
    SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${CMAKE_PROJECT_NAME} ${SRC})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC pulse pulse-simple)
