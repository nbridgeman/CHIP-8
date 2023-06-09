#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <stack>
#include "Timer.hpp"
#include "Memory.hpp"

class CPU {
    public:
        static constexpr uint8_t num_registers = 16;

        CPU();
        ~CPU();
        uint16_t getInstruction(Memory& ram);
        void decode(uint16_t instruction, Memory& ram);

        Timer delay_timer;
        Timer sound_timer;
        uint8_t registers[num_registers] = { 0 };
        uint16_t index_register;
        std::stack<uint16_t> addresses;
        uint16_t program_counter;
        bool super;

    private:
        void opcode00EE(uint16_t instruction);
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
        void opcodeDXYN(uint16_t instruction);
        void opcodeEXNN(uint16_t instruction);
        void opcodeFXNN(uint16_t instruction, Memory ram);
};

#endif