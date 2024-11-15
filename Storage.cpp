#include "Arduino.h"
#include "Debug.h"
#include "Storage.h"
#include "SdFat.h"
SdFat SD;

File Storage::_file;

void Storage::setup() {
  while (!SD.begin(STORAGE_PIN_CS)) {
    DebugLn("SD card missing or not working");
    delay(1000);
  }

  _file = SD.open("/MAIN.dat", O_RDWR | O_CREAT);
  if (!_file) {
    DebugLn("Error opening file on SD card");
    delay(1000);
    while(1);
  }

  _file.flush();
}

void Storage::loop() {

}

uint8_t Storage::getColumn(unsigned long col) {
  if (col < _file.size()) {
    _file.seek(col);
    return _file.read();
  } 
  
  return 0;
}

void Storage::setColumn(unsigned long col, uint8_t value) {
  Debug("setColumn: col=");
  Debug(col);
  Debug(" value=");
  DebugLn(value, BIN);

  _file.seek(col);
  DebugLn(_file.write(value));
  DebugLn(_file.getWriteError());
  _save();
}

void Storage::insertColumn(unsigned long col, uint8_t value = 0) {
  Debug("insertColumn: col=");
  Debug(col);
  Debug(" value=");
  DebugLn(value, BIN);

  uint8_t current;
  uint8_t next = value;
  _file.seek(col);
  while(_file.available()) {
    current = _file.peek();
    _file.write(next);
    next = current;
  }
  _file.write(next);
  _save();
}

void Storage::removeColumn(unsigned long col) {
  Debug("removeColumn: col=");
  DebugLn(col);

  _file.seekEnd();
  bool truncate    = false;
  uint8_t current  = 0;
  uint8_t previous = _file.peek();
  while(_file.position() > max(col, 0)) {
    _file.seekCur(-1);
    current = _file.peek();

    _file.write(previous);
    _file.seekCur(-1);
    previous = current;

    truncate = true;
  }

  if (truncate) {
    _file.seekEnd(-1);
    _file.truncate();
  }

  _save();
}

unsigned long Storage::length() {
  return _file.size();
}

void Storage::_growingSeek(unsigned long col) {
  unsigned long size = _file.size();
  Debug("_growingSeek: size=");
  Debug(size);
  Debug(" col=");
  DebugLn(col);

  if (col < size) {
    _file.seek(col);
    return;
  }

  _file.seek(size);
  for(; size < col; size++) {
    _file.write(byte(0));
  }

  _save();
  _file.seek(col);
}

void Storage::_printFile() {
  Debug("File content:");
  _file.seek(0);
  while(_file.available() > 0){
    Debug(" ");
    Debug(_file.read(), BIN);
  }
  DebugLn("");
}

void Storage::_save() {
  _file.sync();
  _printFile();
}