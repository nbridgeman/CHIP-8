#include "Memory.hpp"

Memory::Memory() { }

Memory::~Memory() { }

void Memory::init() {
    loadFontSet();
}

void Memory::loadFontSet() {
    static constexpr uint8_t num_char = 16;
    static constexpr uint8_t char_size = 5;

    const uint8_t font[num_char][char_size] = { { 0xF0, 0x90, 0x90, 0x90, 0xF0 }, // 0
                                                { 0x20, 0x60, 0x20, 0x20, 0x70 }, // 1
                                                { 0xF0, 0x10, 0xF0, 0x80, 0xF0 }, // 2
                                                { 0xF0, 0x10, 0xF0, 0x10, 0xF0 }, // 3
                                                { 0x90, 0x90, 0xF0, 0x10, 0x10 }, // 4
                                                { 0xF0, 0x80, 0xF0, 0x10, 0xF0 }, // 5
                                                { 0xF0, 0x80, 0xF0, 0x90, 0xF0 }, // 6
                                                { 0xF0, 0x10, 0x20, 0x40, 0x40 }, // 7
                                                { 0xF0, 0x90, 0xF0, 0x90, 0xF0 }, // 8
                                                { 0xF0, 0x90, 0xF0, 0x10, 0xF0 }, // 9
                                                { 0xF0, 0x90, 0xF0, 0x90, 0x90 }, // A
                                                { 0xE0, 0x90, 0xE0, 0x90, 0xE0 }, // B
                                                { 0xF0, 0x80, 0x80, 0x80, 0xF0 }, // C
                                                { 0xE0, 0x90, 0x90, 0x90, 0xE0 }, // D
                                                { 0xF0, 0x80, 0xF0, 0x80, 0xF0 }, // E
                                                { 0xF0, 0x80, 0xF0, 0x80, 0x80 }};// F
    
    for (uint8_t character = 0; character < num_char; character++) {
        for (uint8_t byte = 0; byte < char_size; byte++) {
            write((Memory::font_start + character + byte), font[character][byte]);
        }
    }
}

const uint8_t Memory::read(uint16_t addr) {
    if (addr > Memory::size) {
        // TODO: Turn addr into a hex before printing.
        std::cout << "[Memory] Read Address " << std::hex << unsigned(addr) << " is out of range." << std::endl;
        return 0x00;
    } 
    return mem[addr];
}

void Memory::write(uint16_t addr, uint8_t value) {
    if (addr > Memory::size) {
        // TODO: Turn addr into a hex before printing.
        std::cout << "[Memory] Write Address " << std::hex << unsigned(addr) << " is out of range." << std::endl;
        return;
    }
    mem[addr] = value;
}

uint16_t Memory::getCharacter(uint8_t value) {
    return(mem[Memory::font_start + (value * 5)]);
}