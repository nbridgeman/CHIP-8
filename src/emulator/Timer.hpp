#pragma once

#include <cstdint>

class Timer {
    public:
        Timer();
        ~Timer();
        void write(uint8_t time);
        uint8_t read();
        void decrement();
    private:
        uint8_t value;
};