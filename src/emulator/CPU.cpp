#include "CPU.hpp"

CPU::CPU() { }

CPU::~CPU() { }

uint16_t CPU::getInstruction(Memory& ram) {
    uint16_t instruction = 0x0000;
    instruction = ram.read(program_counter) >> 8 | ram.read(program_counter + 1);
    program_counter += 2;
    return instruction;
}

void CPU::decode(uint16_t instruction) {
    uint16_t first_nibble = instruction & 0xF000;
    switch (first_nibble) {
        case 0x0000:
            opcode00EE(instruction);
            break;
        case 0x1000:
            opcode1NNN(instruction);
            break;
        case 0x2000:
            opcode2NNN(instruction);
            break;
        case 0x3000:
            opcode3XNN(instruction);
            break;
        case 0x4000:
            opcode4XNN(instruction);
            break;
        case 0x5000:
            opcode5XY0(instruction);
            break;
        case 0x9000:
            opcode9XY0(instruction);
            break;
        case 0x6000:
            opcode6XNN(instruction);
            break;
        case 0x7000:
            opcode7XNN(instruction);
            break;
        case 0x8000:
            // operations between VX and VY, create a switch based on nibble 4
            opcode8XYN(instruction);
            break;
        case 0xA000:
            // set index register to NNN
            opcodeANNN(instruction);
            break;
        case 0xB000:
            // Jump to NNN + V0
            opcodeBNNN(instruction);
            break;
        case 0xC000:
            // generates a random number, ands it with NN, puts in VX
            opcodeCXNN(instruction);
            break;
        case 0xD000:
            // display a sprite
            opcodeDXYN(instruction);
            break;
        case 0xE000:
            // different cases, mask for last two nibbles and create a switch statement
            opcodeEXNN(instruction);
            break;
        case 0xF000:
            // lot of different cases, mask for only nibble 3 and 4 and make a switch statement
            opcodeFXNN(instruction);
            break;
    }
}

// If 00EE, exit subroutine. Otherwise do nothing.
void CPU::opcode00EE(uint16_t instruction) {
    uint16_t last_three_nibbles = instruction & 0x0FFF;
    if (last_three_nibbles == 0x00EE) {
        program_counter = addresses.top();
        addresses.pop();
    }
}

// Set PC to NNN
void CPU::opcode1NNN(uint16_t instruction) {
    program_counter = (instruction & 0x0FFF);
}

// push address onto stack, enter subroutine
void CPU::opcode2NNN(uint16_t instruction) {
    uint16_t last_three_nibbles = instruction & 0x0FFF;
    addresses.push(last_three_nibbles);
    program_counter = last_three_nibbles;
}

// skip if VX == NN
void CPU::opcode3XNN(uint16_t instruction) {
    uint16_t register1 = (instruction & 0x0F00) >> 8;
    uint16_t value = instruction & 0x00FF;
    if (registers[register1] == value) {
        program_counter += 2;
    }
}

// skip if VX != NN
void CPU::opcode4XNN(uint16_t instruction) {
    uint16_t register1 = (instruction & 0x0F00) >> 8;
    uint16_t value = instruction & 0x00FF;
    if (registers[register1] != value) {
        program_counter += 2;
    }
}

// skip if VX == VY
void CPU::opcode5XY0(uint16_t instruction) {
    uint16_t register1 = (instruction & 0x0F00) >> 8;
    uint16_t register2 = (instruction & 0x00F0) >> 4;
    if (registers[register1] == registers[register2]) {
        program_counter += 2;
    }
}

// skip if VX != VY
void CPU::opcode9XY0(uint16_t instruction) {
    uint16_t register1 = (instruction & 0x0F00) >> 8;
    uint16_t register2 = (instruction & 0x00F0) >> 4;
    if (registers[register1] != registers[register2]) {
        program_counter += 2;
    }
}

// sets VX = NN
void CPU::opcode6XNN(uint16_t instruction) {
    uint16_t register1 = (instruction & 0x0F00) >> 8;
    uint16_t value = instruction & 0x00FF;
    registers[register1] = value;
}

// sets VX += NN
void CPU::opcode7XNN(uint16_t instruction) {
    uint16_t register1 = (instruction & 0x0F00) >> 8;
    uint16_t value = instruction & 0x00FF;
    uint16_t sum = registers[register1] + value;
    if (sum > 255) {
        registers[0xF] = 1;
    }
    registers[register1] = sum;
}
