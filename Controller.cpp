#include "Arduino.h"
#include "Debug.h"
#include "Storage.h"
#include "Controller.h"
#include "Display.h"
#include "Buttons.h"

static const uint8_t _RELAIS_PIN_DATA[8] = { CONTROLLER_RELAIS_PINS };
const uint8_t* Controller::_RELAIS_PINS = _RELAIS_PIN_DATA;

uint32_t Controller::_cursor;
bool Controller::_isPlaying = false;
unsigned long Controller::_nextMillis;

void Controller::setup() {
  _cursor = 0;

  for (uint8_t i = 0; i < 8; i++) {
    pinMode(_RELAIS_PINS[i], OUTPUT);
  }

  _refreshDisplay();
  _refreshRelais();
  _setPlaying(true);
}

void Controller::loop() {
  _handleButtons();
  _handlePlaying();
}

void Controller::_handleButtons() {
  uint8_t input = Buttons::getInput();
  if (!input) {
    return;
  }

  if (_isPlaying) {
    _setPlaying(false);
    return;
  }

  // handle channel inputs
  if (Buttons::INPUT_CHANNEL_0 <= input && input <= Buttons::INPUT_CHANNEL_7) {
    uint8_t before = Storage::getColumn(_cursor);
    uint8_t after  = before ^ ( 1 << ( 8 - input));
    Debug("Handle Button: input=");
    Debug(input);
    Debug(", before=");
    Debug(before, BIN);
    Debug(", after=");
    Debug(after, BIN);
    Debug(", cursor=");
    DebugLn(_cursor);
    Storage::setColumn(_cursor, after);
    _refreshDisplay();
    return;
  }

  // handle control inputs
  switch(input) {
    case Buttons::INPUT_PREV_COL:
      DebugLn("Handle Button: INPUT_PREV_COL");
      if (_cursor == 0) break;
      _cursor -= 1;
      _refreshDisplay();
      break;

    case Buttons::INPUT_PREV_BLOCK:
      DebugLn("Handle Button: INPUT_PREV_BLOCK");
      if (_cursor == 0) break;
      _cursor -= min(_cursor, 8);
      _refreshDisplay();
      break;

    case Buttons::INPUT_NEXT_COL:
      DebugLn("Handle Button: INPUT_NEXT_COL");
      _cursor = min(_cursor + 1, Storage::length());
      _refreshDisplay();
      break;

    case Buttons::INPUT_NEXT_BLOCK:
      DebugLn("Handle Button: INPUT_NEXT_BLOCK");
      _cursor = min(_cursor + 8, Storage::length());
      _refreshDisplay();
      break;

    case Buttons::INPUT_FIRST_COL:
      DebugLn("Handle Button: INPUT_FIRST_COL");
      _cursor = 0;
      _refreshDisplay();
      break;

    case Buttons::INPUT_LAST_COL:
      DebugLn("Handle Button: INPUT_LAST_COL");
      _cursor = Storage::length() - 1;
      _refreshDisplay();
      break;

    case Buttons::INPUT_NEW_COL:
      DebugLn("Handle Button: INPUT_NEW_COL");
      if (_cursor < Storage::length()) {
        uint8_t column = Storage::getColumn(_cursor);
        Storage::insertColumn(_cursor + 1, column);
      }
      _cursor++;
      _refreshDisplay();
      break;
    
    case Buttons::INPUT_DEL_COL:
      DebugLn("Handle Button: INPUT_DEL_COL");
      if (_cursor < Storage::length()) {
        Storage::removeColumn(_cursor);
      }
      if (_cursor > 0) {
        _cursor--;
      }
      _refreshDisplay();
      break;
    
    case Buttons::INPUT_PLAY:
      DebugLn("Handle Button: INPUT_PLAY");
      _setPlaying(true);
      break;

    default:
      Debug("Invalid Button on _handleButtons: ");
      DebugLn(input, BIN);
  }
}

void Controller:: _handlePlaying() {
  if (!_isPlaying || millis() < _nextMillis) {
    return; 
  }

  DebugLn("_handlePlaying");
  _cursor++;
  if (_cursor >= Storage::length()) {
    _cursor = 0;
  }

  _refreshDisplay();
  _nextMillis = millis() + _durationInMs();
}

void Controller::_refreshDisplay() { 
  for (int displayCol = 0; displayCol < DISPLAY_COLS_ALL; displayCol++) {
    int32_t dataCol = _cursor - DISPLAY_CURSOR_COL + displayCol;
    uint8_t value = 0;
    if (dataCol >= 0 && dataCol < Storage::length()) {
      value = Storage::getColumn(dataCol);
    }

    Display::setCol(displayCol, value);
  }
  _refreshRelais();
}

void Controller::_refreshRelais() {
  uint8_t state = Storage::getColumn(_cursor);
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(_RELAIS_PINS[i], bitRead(~state, i));
  }
}

void Controller::_setPlaying(bool isPlaying) {
  Debug("_setPlaying: isPlaying=");
  DebugLn(isPlaying);

  if (_isPlaying == isPlaying) {
    return;
  }

  if (!isPlaying) {
    _isPlaying = false;
    return;
  }

  _isPlaying = true;
  _nextMillis = millis();
}

uint32_t Controller::_durationInMs() {
  return 500; // 0,5 seconds
}