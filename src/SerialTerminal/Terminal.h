#ifndef _TERMINAL_H
#define _TERMINAL_H

#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <cstdint>
#include <cstdio>
#include <list>

namespace SerialTerminal {

	

using std::string;
using namespace std;

class Terminal;

using funcPtr = function<void(Terminal *, string)>;

class Terminal {
private:
  string _buffer;
  map<string, funcPtr> _callbacksMap;

  void _receivePartOfPackage(uint8_t part);
  void _insertToBuffer(uint8_t part);
  void _callAndClearBuffer();
  void _call();
  void _clearBuffer();
  string _getEvent();

public:
  void write(uint8_t);
  void attach(string, funcPtr);
  void detach(string);

  list<string> getEvents();
};

} // namespace SerialTerminal

#endif