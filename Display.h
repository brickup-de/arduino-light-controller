#ifndef Display_h
#define Display_h
//##########################################################
#define DISPLAY_PIN_DIN A1
#define DISPLAY_PIN_CS A2
#define DISPLAY_PIN_CLK A3

#define DISPLAY_COUNT 4
#define DISPLAY_ROWS 8
#define DISPLAY_COLS 8
#define DISPLAY_COLS_ALL 32
#define DISPLAY_CURSOR_COL 8
#define DISPLAY_INTENSITY 0
//##########################################################
#include "Arduino.h"
#include <LedControl.h>
//##########################################################

class Display {
public:
  static void setup();
  static void loop();

  static void setRow(uint8_t row, uint32_t value);
  static void setCol(uint8_t col, uint8_t value);
  static void clear();

private:
  static LedControl _ledMatrix;
};

#endif
