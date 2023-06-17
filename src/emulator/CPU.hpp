#pragma once

#include <cstdint>
#include <stack>
#include <iostream>
#include "Timer.hpp"
#include "Memory.hpp"
#include "Display.hpp"

class CPU {
    public:
        static constexpr uint8_t num_registers = 16;

        CPU();
        ~CPU();
        uint16_t getInstruction(Memory& ram);
        void decode(uint16_t instruction, Memory& ram, Display& out);
        void updateTimers();

        Timer delay_timer;
        Timer sound_timer;
        uint8_t registers[num_registers] = { 0 };
        uint16_t index_register;
        std::stack<uint16_t> addresses;
        uint16_t program_counter;
        bool super;

    private:
        bool last_keys[16] = { false };
        void opcode00EE(uint16_t instruction, Display& display);
        void opcode1NNN(uint16_t instruction);
        void opcode2NNN(uint16_t instruction);
        void opcode3XNN(uint16_t instruction);
        void opcode4XNN(uint16_t instruction);
        void opcode5XY0(uint16_t instruction);
        void opcode9XY0(uint16_t instruction);
        void opcode6XNN(uint16_t instruction);
        void opcode7XNN(uint16_t instruction);
        void opcode8XYN(uint16_t instruction);
        void opcodeANNN(uint16_t instruction);
        void opcodeBNNN(uint16_t instruction);
        void opcodeCXNN(uint16_t instruction);
        void opcodeDXYN(uint16_t instruction, Display& display, Memory& ram);
        void opcodeEXNN(uint16_t instruction, Display& display);
        void opcodeFXNN(uint16_t instruction, Memory& ram, Display& display);
};