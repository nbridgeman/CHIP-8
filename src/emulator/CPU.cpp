#include "CPU.hpp"
#include <iomanip>

CPU::CPU() { }

CPU::~CPU() { }

uint16_t CPU::getInstruction(Memory& ram) {
    uint16_t instruction = 0x0000;
    instruction = ram.read(program_counter) << 8 | ram.read(program_counter + 1);
    program_counter += 2;
    // std::cout << std::hex << instruction << std::endl;
    return instruction;
}

void CPU::decode(uint16_t instruction, Memory& ram, Display& out) {
    uint16_t first_nibble = instruction & 0xF000;
    switch (first_nibble) {
        case 0x0000:
            opcode00EE(instruction, out);
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
            opcodeDXYN(instruction, out, ram);
            break;
        case 0xE000:
            // different cases, mask for last two nibbles and create a switch statement
            opcodeEXNN(instruction, out);
            break;
        case 0xF000:
            // lot of different cases, mask for only nibble 3 and 4 and make a switch statement
            opcodeFXNN(instruction, ram, out);
            break;
        default:
            std::cout << "[Opcode] Opcode " << std::hex << unsigned(instruction) << " is not a valid opcode." << std::endl;
            break;
    }
}

void CPU::updateTimers() {
    delay_timer.decrement();
    sound_timer.decrement();
}

// If 00EE, exit subroutine. Otherwise do nothing.
void CPU::opcode00EE(uint16_t instruction, Display& display) {
    uint16_t last_three_nibbles = instruction & 0x0FFF;
    if (last_three_nibbles == 0x00EE) {
        program_counter = addresses.top();
        addresses.pop();
    } else if (last_three_nibbles == 0x00E0) {
        display.clear();
    }
}

// Set PC to NNN (jump)
void CPU::opcode1NNN(uint16_t instruction) {
    program_counter = (instruction & 0x0FFF);
}

// push address onto stack, enter subroutine
void CPU::opcode2NNN(uint16_t instruction) {
    uint16_t last_three_nibbles = instruction & 0x0FFF;
    addresses.push(program_counter);
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
    registers[register1] = registers[register1] + value;
}

// Logical operations and arithmetic
void CPU::opcode8XYN(uint16_t instruction) {
    uint16_t register1 = (instruction & 0x0F00) >> 8;
    uint16_t register2 = (instruction & 0x00F0) >> 4;
    uint16_t last_nibble = (instruction & 0x000F);
    uint8_t flag = 0;
    switch(last_nibble) {
        case 0x0:
            registers[register1] = registers[register2];
            break;
        case 0x1:
            registers[register1] = registers[register1] | registers[register2];
            break;
        case 0x2:
            registers[register1] = registers[register1] & registers[register2];
            break;
        case 0x3:
            registers[register1] = registers[register1] ^ registers[register2];
            break;
        case 0x4:
            if (unsigned(registers[register1]) + unsigned(registers[register2] > 255)) {
                flag = 1;
            }
            registers[register1] = registers[register1] + registers[register2];
            registers[0x0F] = flag;
            break;
        case 0x5:
            if (registers[register1] > registers[register2]) {
                flag = 1;
            }
            registers[register1] = registers[register1] - registers[register2];
            registers[0x0F] = flag;
            break;
        case 0x7:
            if (registers[register2] > registers[register1]) {
                flag = 1;
            }
            registers[register1] = registers[register2] - registers[register1];
            registers[0x0F] = flag;
        case 0x6:
            if (super) {
                registers[register1] = registers[register2];
            }
            if (registers[register1] % 2 != 0) {
                flag = 1;
            }
            registers[register1] = registers[register1] >> 1;
            registers[0x0F] = flag;
            break;
        case 0xE:
            if (super) {
                registers[register1] = registers[register2];
            }
            if (registers[register1] >= 128) {
                flag = 1;
            }
            registers[register1] = registers[register1] << 1;
            registers[0x0F] = flag;
            break;
        default:
            std::cout << "[Opcode] Opcode " << std::hex << unsigned(instruction) << " is not a valid opcode." << std::endl;
            break;
    }
}

// set index
void CPU::opcodeANNN(uint16_t instruction) {
    uint16_t last_three_nibbles = instruction & 0x0FFF;
    index_register = last_three_nibbles;
}

// jump with offset
void CPU::opcodeBNNN(uint16_t instruction) {
    uint16_t last_three_nibbles = instruction & 0x0FFF;
    if (super) {
        uint16_t register1 = (instruction & 0x0F00) >> 8;
        uint16_t new_address = last_three_nibbles + registers[register1];
        if (new_address > 0xFFF) {
            std::cout << "[Opcode] Address " << new_address << " is not a valid address." << std::endl;
        } else {
            program_counter = new_address;
        }
    } else {
        program_counter = last_three_nibbles + registers[0];
    }
}

// Generates a random numbers, ands it with NN, and stores in register X
void CPU::opcodeCXNN(uint16_t instruction) {
    uint16_t register1 = (instruction & 0x0F00) >> 8;
    uint16_t value = instruction & 0x00FF;
    srand(time(0));
    uint8_t rand_value = rand() % 256;
    registers[register1] = rand_value && value;
    return;
}

// Update display with sprite found at index register
void CPU::opcodeDXYN(uint16_t instruction, Display& display, Memory& ram) {
    uint16_t register1 = (instruction & 0x0F00) >> 8;
    uint16_t register2 = (instruction & 0x00F0) >> 4;
    uint16_t last_nibble = (instruction & 0x000F);
    registers[0x0F] = display.draw(registers[register1], registers[register2], last_nibble, ram, index_register);
    return;
}

// Skip if key is/is not pressed
void CPU::opcodeEXNN(uint16_t instruction, Display& display) {
    uint16_t register1 = (instruction & 0x0F00) >> 8;
    uint16_t operation = instruction & 0x00FF;
    if (operation == 0x009E) {
        if (display.keyIsPressed && display.keyPressed == registers[register1]) {
            program_counter += 2;
        }
    } else if (operation == 0x00A1) {
        if (display.keyIsPressed && display.keyPressed != registers[register1]) {
            program_counter += 2;
        }
    }
}

// Timers and others
void CPU::opcodeFXNN(uint16_t instruction, Memory ram, Display& display) {
    uint16_t register1 = (instruction & 0x0F00) >> 8;
    uint16_t operation = instruction & 0x00FF;
    switch (operation) {
        // set register to delay_timer
        case 0x07:
            registers[register1] = delay_timer.read();
            break;
        // set delay_timer to register
        case 0x15:
            delay_timer.write(registers[register1]);
            break;
        // set sound_timer to register
        case 0x18:
            sound_timer.write(registers[register1]);
            break;
        // add register to index register
        case 0x1E:
            index_register += registers[register1];
            index_register &= 0xFFF;
        // decrements PC until a key is pressed
        case 0x0A:
            if (!display.keyIsPressed || display.keyPressed != registers[register1]) {
                program_counter -= 2;
            }
            break;
        // gets the location of font character
        case 0x29:
            index_register = ram.getCharacter(registers[register1]);
            break;
        // binary-coded decimal conversion
        case 0x33:
            {
                uint8_t value = registers[register1];
                for (uint8_t iter = 0; iter < 3; iter++) {
                    ram.write((index_register + (2 - iter)), value % 10);
                    value = value / 10;
                }
                break;
            }
        // Store memory
        case 0x55:
            if (super) {
                for (uint8_t reg = 0; reg <= register1; reg++) {
                    ram.write((index_register + reg), registers[reg]);
                }
            } else {
                for (uint8_t reg = 0; reg <= register1; reg++) {
                    ram.write((index_register), registers[reg]);
                    index_register++;
                }
            }
            break;
        // Load memory
        case 0x65:
            if (super) {
                for (uint8_t reg = 0; reg <= register1; reg++) {
                    registers[reg] = ram.read(index_register + reg);
                }
            } else {
                for (uint8_t reg = 0; reg <= register1; reg++) {
                    registers[reg] = ram.read(index_register);
                    index_register++;
                }
            }
            break;
        default:
            std::cout << "[Opcode] Opcode " << std::hex << unsigned(instruction) << " is not a valid opcode." << std::endl;
            break;
    }
}