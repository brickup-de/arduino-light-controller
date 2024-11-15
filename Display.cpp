#include "Arduino.h"
#include "Debug.h"
#include "Display.h"

LedControl Display::_ledMatrix = LedControl(DISPLAY_PIN_DIN,
                                           DISPLAY_PIN_CLK,
                                           DISPLAY_PIN_CS,
                                           DISPLAY_COUNT);

void Display::setup() {
  for (uint8_t i = 0; i < DISPLAY_COUNT; i++) {
    _ledMatrix.clearDisplay(i);
    _ledMatrix.setIntensity(i, DISPLAY_INTENSITY);
    _ledMatrix.shutdown(i, false);
  }
}


void Display::loop() {
}


void Display::setRow(uint8_t row, uint32_t value) {
  for (uint8_t addr = 0; addr < DISPLAY_COUNT; addr++) {
    uint8_t val = value >> (addr * DISPLAY_COLS);
    _ledMatrix.setRow(addr, row, val);
  }
}


void Display::setCol(uint8_t col, uint8_t value) {
  int _col = col % DISPLAY_COLS;
  int _addr = DISPLAY_COUNT - 1 - col / DISPLAY_COLS;
  _ledMatrix.setColumn(_addr, _col, value);
}


void Display::clear() {
  for (uint8_t addr = 0; addr < DISPLAY_COUNT; addr++) {
    _ledMatrix.clearDisplay(addr);
  }
}