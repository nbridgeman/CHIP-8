cmake_minimum_required(VERSION 3.21)
project(chip8 LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML 
    GIT_REPOSITORY https://github.com/SFML/SFML.git 
    GIT_TAG 2.6.X)
FetchContent_MakeAvailable(SFML)

add_executable(chip8 src/main.cpp src/emulator/Emulator.cpp src/emulator/Display.cpp src/emulator/CPU.cpp src/emulator/Memory.cpp src/emulator/Timer.cpp)

target_link_libraries(chip8 PRIVATE sfml-graphics)
target_compile_features(chip8 PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET chip8 POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:chip8> $<TARGET_FILE_DIR:chip8> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS chip8)