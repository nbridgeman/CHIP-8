#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Memory.hpp"

class Display {
    public:
        Display();
        ~Display();
        bool isOpen();
        void open();
        uint8_t draw(uint8_t x, uint8_t y, uint8_t n, Memory& ram, uint16_t index);
        void clear();
        void updateDisplay();
        void getInput();
        uint8_t keyPressed;
        bool keyIsPressed;
    private:
        bool pixels[2048] = { 0 };
        uint16_t getLoc(uint8_t x, uint8_t y);
        uint8_t getX(uint16_t loc);
        uint8_t getY(uint16_t loc);
        sf::RenderWindow window;
};