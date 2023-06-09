#include "../../include/Timer.hpp"

Timer::Timer() {
    value = 0;
}

Timer::~Timer() { }

void Timer::write(uint8_t time) {
    value = time;
} 

uint8_t Timer::read() {
    return value;
}

void Timer::decrement() {
    if (value > 0) {
        value--;
    }
}