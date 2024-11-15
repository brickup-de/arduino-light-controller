#ifndef Controller_h
#define Controller_h
//##########################################################
#define CONTROLLER_COLS_BEFORE_NOW 8
#define CONTROLLER_COLS_AFTER_NOW 15
#define CONTROLLER_RELAIS_PINS 2,3,4,5,6,7,8,9
//##########################################################
#include "Arduino.h"
//##########################################################

class Controller {
public:
  static void setup();
  static void loop();

private:
  static const uint8_t* _RELAIS_PINS;
  static uint32_t _cursor;
  static bool _isPlaying;
  static unsigned long _nextMillis;

  static void _handleButtons();
  static void _handlePlaying();

  static void _refreshDisplay();
  static void _refreshRelais();
  static void _setPlaying(bool isPlaying);
  static uint32_t _durationInMs();
};

#endif
