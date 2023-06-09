cmake_minimum_required(VERSION 3.0.0)
project(chip8)
include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_executable(chip8 ${SOURCES})

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
