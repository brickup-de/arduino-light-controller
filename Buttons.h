#ifndef Buttons_h
#define Buttons_h
//##########################################################
#define BUTTON_CHANNEL_PIN A0
#define BUTTON_PREV_PIN A4
#define BUTTON_NEXT_PIN A5

#define BUTTON_THRESHOLD_LIST 35, 140, 237, 313, 455, 610, 760, 935, 1024
#define BUTTON_THRESHOLD_BOOL 512 
#define BUTTON_DEBOUNCE_DELAY_IN_MS 20
#define BUTTON_LONGPRESS_MAX_MS 1000
#define BUTTON_LONGPRESS_MIN_MS 500
#define BUTTON_LONGPRESS_FACTOR 0.8
//##########################################################
#include "Arduino.h"
//##########################################################

class Buttons {
  public:
    // fixed numbers as mapped onto the BUTTON_THRESHOLD_LIST-indices
    static const uint8_t INPUT_NONE       = 0;
    static const uint8_t INPUT_CHANNEL_0  = 1;
    static const uint8_t INPUT_CHANNEL_1  = 2;
    static const uint8_t INPUT_CHANNEL_2  = 3;
    static const uint8_t INPUT_CHANNEL_3  = 4;
    static const uint8_t INPUT_CHANNEL_4  = 5;
    static const uint8_t INPUT_CHANNEL_5  = 6;
    static const uint8_t INPUT_CHANNEL_6  = 7;
    static const uint8_t INPUT_CHANNEL_7  = 8;

    // flexible commands mapped onto extra buttons / long presses
    static const uint8_t INPUT_PREV_COL   = 9;
    static const uint8_t INPUT_PREV_BLOCK = 10;
    static const uint8_t INPUT_NEXT_COL   = 11;    
    static const uint8_t INPUT_NEXT_BLOCK = 12;
    static const uint8_t INPUT_FIRST_COL  = 13;    
    static const uint8_t INPUT_LAST_COL   = 14;
    static const uint8_t INPUT_DEL_COL    = 15;
    static const uint8_t INPUT_NEW_COL    = 16;
    static const uint8_t INPUT_PLAY       = 17;

    static void setup();
    static void loop();

    static bool hasInput();
    static uint8_t getInput();
  
  private:
    struct state {
      uint8_t prevRead;
      uint8_t thisRead;
      uint8_t debouncedRead;
      uint8_t longPressCount;
      unsigned long readChangeAt;
      unsigned long debouncedReadAt;
      uint8_t shortPressed;
      uint8_t longPressed;
    };

    static struct state _channels;
    static struct state _next;
    static struct state _prev;

    static void _refreshState(uint8_t pin, struct state& state, bool as_bool);
    static uint8_t _read(uint8_t pin, bool as_bool);
};

#endif