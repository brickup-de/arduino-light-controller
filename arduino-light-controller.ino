#include "Debug.h"
#include "Buttons.h"
#include "Display.h"
#include "Storage.h"
#include "Controller.h"

void setup() {
  initDebuggung();
  Display::setup();
  Buttons::setup();
  Storage::setup();
  Controller::setup();
}

void loop() {
  Buttons::loop();
  Display::loop();
  Storage::loop();
  Controller::loop();
}