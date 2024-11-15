#ifndef Storage_h
#define Storage_h
//##########################################################
#define STORAGE_PIN_CS 10
// STORAGE_PIN_MOSI must be 11 on Arduino Uno
// STORAGE_PIN_MISO must be 12 on Arduino Uno
// STORAGE_PIN_SCK  must be 13 on Arduino Uno
//##########################################################
#include "Arduino.h"
#include <SPI.h>
#include "SdFat.h"
//##########################################################

class Storage {
public:
  static void setup();
  static void loop();

  static uint8_t getColumn(unsigned long col);
  static void setColumn(unsigned long col, uint8_t newValue);

  static void insertColumn(unsigned long col, uint8_t insertValue);
  static void removeColumn(unsigned long col);

  static unsigned long length();

  static void test();

private:
  static File _file;
  static uint8_t _sequence;

  static void _growingSeek(unsigned long col);
  static uint8_t _emptyCol();
  static void _printFile();
  static void _save();
};

#endif
