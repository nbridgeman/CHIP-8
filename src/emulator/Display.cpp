#include "Display.hpp"

Display::Display() {

}

Display::~Display() {

}

bool Display::isOpen() {
    return window.isOpen();
}

void Display::open() {
    window.create(sf::VideoMode(640, 320), "CHIP-8 Emulator");
}

uint8_t Display::draw(uint8_t x, uint8_t y, uint8_t n, Memory& ram, uint16_t index) {
    uint8_t flag = 0;
    for (uint8_t row = 0; row < n; row++) {
        uint8_t bytes = ram.read(index + row);
        for (uint8_t col = 0; col < 8; col++) {
            uint16_t loc = getLoc(x + col, y + row);
            pixels[loc] = pixels[loc] ^ ((bytes >> (8 - col - 1)) & 0x01);
        }
    }
    updateDisplay();
    return flag;
}

uint16_t Display::getLoc(uint8_t x, uint8_t y) {
    uint16_t res = 0;
    res += (x % 64);
    res += (y % 32) * 64;
    return res;
}

void Display::clear() {
    for (int i = 0; i < 2048; i++) {
        pixels[i] = 0;
    }
}

void Display::updateDisplay() {
    sf::Event event;
    while (window.pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
            window.close();
    }

    // clear the window with black color
    window.clear(sf::Color::Black);

    // draw everything here...
    for (uint16_t pixel = 0; pixel < 2048; pixel++) {
        if (pixels[pixel]) {
            sf::RectangleShape on_pixel(sf::Vector2(10.f, 10.f));
            on_pixel.setFillColor(sf::Color::White);
            on_pixel.setPosition(sf::Vector2(float(getX(pixel) * 10), float(getY(pixel) * 10)));
            window.draw(on_pixel);
        }
    }

    // end the current frame
    window.display();
}

uint8_t Display::getX(uint16_t loc) {
    return loc % 64;
}

uint8_t Display::getY(uint16_t loc) {
    return loc / 64;
}