#pragma once

#include <cstdint>
#include <chrono>
#include <string>
#include <fstream>
#include <iostream>
#include "CPU.hpp"
#include "Memory.hpp"
#include "Display.hpp"

class Emulator {
    public:
        Emulator();
        ~Emulator();
        void loadRom(std::string fileName);                 // loads contents of a file into Memory
        void run();                                         // calls the main loop
    private:
        void doInstruction();                                // calls loadInstruction, decode, and execute 
        Memory ram;
        Display out; 
        CPU cpu;
};