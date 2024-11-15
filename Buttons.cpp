#include "Arduino.h"
#include "Debug.h"
#include "Buttons.h"

struct Buttons::state Buttons::_channels = {};
struct Buttons::state Buttons::_next = {};
struct Buttons::state Buttons::_prev = {};

void Buttons::setup() {
  pinMode(BUTTON_CHANNEL_PIN, INPUT);
  pinMode(BUTTON_NEXT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PREV_PIN, INPUT_PULLUP);

  unsigned long now = millis();
  _channels.debouncedReadAt = now;
  _next.debouncedReadAt = now;
  _prev.debouncedReadAt = now;
}

void Buttons::loop() {
  _refreshState(BUTTON_CHANNEL_PIN, _channels, false);
  _refreshState(BUTTON_NEXT_PIN, _next, true);
  _refreshState(BUTTON_PREV_PIN, _prev, true);
}

bool Buttons::hasInput() {
  return Buttons::getInput() != Buttons::INPUT_NONE;
}

uint8_t Buttons::getInput() {
  // Navigation
  if (_prev.shortPressed) {
    return Buttons::INPUT_PREV_COL;
  }
  if (_prev.longPressed) {
    return Buttons::INPUT_PREV_BLOCK;
  }
  if (_next.shortPressed) {
    return Buttons::INPUT_NEXT_COL;
  }
  if (_next.longPressed) {
    return Buttons::INPUT_NEXT_BLOCK;
  }

  // Channels short press
  if (_channels.shortPressed) {
    return _channels.shortPressed;
  }

  // Channels with single trigger on longpress
  if (_channels.longPressCount == 1) {
    switch (_channels.longPressed) {
      case Buttons::INPUT_CHANNEL_0:
        return Buttons::INPUT_FIRST_COL;
      case Buttons::INPUT_CHANNEL_2:
        return Buttons::INPUT_PLAY;
      case Buttons::INPUT_CHANNEL_7:
        return Buttons::INPUT_LAST_COL;
    }
  }

  // Channels with repeated trigger on longpress
  switch (_channels.longPressed) {
    case Buttons::INPUT_CHANNEL_4:
      return Buttons::INPUT_DEL_COL;
    case Buttons::INPUT_CHANNEL_5:
      return Buttons::INPUT_NEW_COL;
  }

  // Nothing happend if we get here
  return Buttons::INPUT_NONE;
}

void Buttons::_refreshState(uint8_t pin, struct state& state, bool as_bool) {
  state.shortPressed = Buttons::INPUT_NONE;
  state.longPressed  = Buttons::INPUT_NONE; 
  state.prevRead     = state.thisRead;
  state.thisRead     = _read(pin, as_bool);

  // handle noisy changes of input (prepare debounce)
  if (state.thisRead != state.prevRead) {
    state.readChangeAt = millis();
    return;
  }
  
  // handle stable changes of input (debounce)
  if (state.thisRead != state.debouncedRead) {
    unsigned long stableReadDuration = millis() - state.readChangeAt;
    if (stableReadDuration > BUTTON_DEBOUNCE_DELAY_IN_MS) {
      state.shortPressed    = state.longPressCount ? Buttons::INPUT_NONE : state.debouncedRead;
      state.debouncedRead   = state.thisRead;
      state.debouncedReadAt = millis();
      state.longPressCount  = 0;
    }
    return;
  }

  // handle stable long press of input
  if (state.debouncedRead) {
    uint16_t longpressDelay = BUTTON_LONGPRESS_MAX_MS * pow(BUTTON_LONGPRESS_FACTOR, state.longPressCount);
    longpressDelay = max(longpressDelay, BUTTON_LONGPRESS_MIN_MS);

    unsigned long stablePressDuration = millis() - state.debouncedReadAt;
    if (stablePressDuration >= longpressDelay) {
      state.longPressed     = state.debouncedRead;
      state.longPressCount  = min(state.longPressCount + 1, 20);
      state.debouncedReadAt = millis();
    }
  }
}

uint8_t Buttons::_read(uint8_t pin, bool as_bool) {
  int reading = analogRead(pin);
  if (as_bool) {
    return reading >= BUTTON_THRESHOLD_BOOL ? 0 : 255;
  }

  const uint16_t THRESHOLDS[9] = { BUTTON_THRESHOLD_LIST };
  for (uint8_t i = 0; i <= 8; i++) {
    if (reading <= THRESHOLDS[i]) {
      return i;
    }
  }
}