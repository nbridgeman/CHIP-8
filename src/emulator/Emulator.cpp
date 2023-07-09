#include "Emulator.hpp"

Emulator::Emulator() {

}

Emulator::~Emulator() {

}

void Emulator::loadRom(std::string fileName) {
    std::streampos size;
    char * memblock;

    std::ifstream file (fileName, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open()) {
        size = file.tellg();
        memblock = new char[size];
        file.seekg(0, std::ios::beg);
        file.read(memblock, size);
        file.close();

        for (int i = 0; i < size; i++) {
            ram.write(ram.program_start + i, memblock[i]);
        }

        delete[] memblock;
        std::cout << "ROM loaded" << std::endl;
    }
}

void Emulator::run() {
    // every 16.67 ms we do a bunch of instructions and decrement timers
    using namespace std;
    using chrono::steady_clock;
    ram.loadFontSet();
    cpu.program_counter = ram.program_start;
    out.open();
    steady_clock::time_point start = steady_clock::now();
    while (out.isOpen()) {
        steady_clock::time_point curr = steady_clock::now();
        if (chrono::duration_cast<chrono::milliseconds>(curr - start).count() >= 17) {
            uint16_t instruction_num = 15;
            while (instruction_num > 0 && cpu.program_counter < ram.size) {
                doInstruction();
                instruction_num--;
            }
            cpu.updateTimers();
            out.updateDisplay();
            start = steady_clock::now();
        }
    }
}

void Emulator::doInstruction() {
    cpu.decode(cpu.getInstruction(ram), ram, out);
}