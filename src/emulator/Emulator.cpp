#include "Emulator.hpp"

Emulator::Emulator() {

}

Emulator::~Emulator() {

}

void Emulator::loadRom(std::string fileName) {
    ram.init();
    std::ifstream fs(fileName);
    uint8_t op = fs.get();
    uint16_t addr = ram.program_start;
    while(fs && addr < ram.size) {
        ram.write(addr, op);
        addr++;
        op = fs.get();
    }
    std::cout << "ROM loaded" << std::endl;
}

void Emulator::run() {
    // every 16.67 ms we do a bunch of instructions and decrement timers
    using namespace std;
    using chrono::steady_clock;
    cpu.program_counter = ram.program_start;
    out.open();
    steady_clock::time_point start = steady_clock::now();
    while (out.isOpen()) {
        steady_clock::time_point curr = steady_clock::now();
        if (chrono::duration_cast<chrono::milliseconds>(curr - start).count() >= 17) {
            uint16_t instruction_num = 1;
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