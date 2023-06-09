#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <iostream>

class Memory {
public:
    static constexpr uint16_t size = 4096;
    static constexpr uint16_t font_start = 0x0050;
    static constexpr uint16_t program_start = 0x0200;

    Memory();    
    ~Memory();
    void init();
    const uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t value);
    void loadFontSet();
    uint16_t getCharacter(uint8_t value);
private:
    uint8_t mem[Memory::size] = { 0 };
};


#endif
