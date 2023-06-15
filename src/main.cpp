#include "./emulator/Emulator.hpp"

int main() {
   Emulator chip8;
   chip8.loadRom("../roms/3-corax+.ch8");
   chip8.run();

   return 0;
}