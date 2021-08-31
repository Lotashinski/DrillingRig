#include "Terminal.h"
#include <cstdint>
#include <cstdio>
#include <list>
#include <string>

using namespace SerialTerminal;

static const unsigned char CL = 0x0D;
static const unsigned char DEL = 0x7F;

void Terminal::write(uint8_t p) {
  if (p == 0x00)
    p = CL;
  _receivePartOfPackage(p);
}

void Terminal::attach(string event, funcPtr callback) {
  _callbacksMap[event] = callback;
}

void Terminal::detach(string event) {
  auto it = _callbacksMap.find(event);
  _callbacksMap.erase(it);
}

list<string> Terminal::getEvents() {
  list<string> events;
  for (auto const &it : _callbacksMap)
    events.push_back(it.first);
  return events;
}

void Terminal::_receivePartOfPackage(uint8_t partOfPackage) {
  if (partOfPackage != CL) // end-of-line check
    _insertToBuffer(partOfPackage);
  else
    _callAndClearBuffer();
}

void Terminal::_insertToBuffer(uint8_t partOfPackage) {
  _buffer += partOfPackage;
}

void Terminal::_callAndClearBuffer() {
  _call();
  _clearBuffer();
}

void Terminal::_call() {
  auto event = _getEvent();
  auto it = _callbacksMap.find(event);
  if (it != _callbacksMap.end())
    it->second(this, _buffer);
}

void Terminal::_clearBuffer() { _buffer = ""; }

string Terminal::_getEvent() {
  auto delim = " ";
  auto pos = _buffer.find(delim);
  return _buffer.substr(0, (pos == string::npos) ? _buffer.length() : pos);
}