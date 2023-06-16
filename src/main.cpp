#include "./emulator/Emulator.hpp"

int main() {
   Emulator chip8;
   chip8.loadRom("../roms/6-keypad.ch8");
   chip8.run();

   return 0;
}