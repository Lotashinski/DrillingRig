#ifndef _TERMINAL_CONTROLLER_H
#define _TERMINAL_CONTROLLER_H

#include "mbed.h"
#include "src/Measurement/Buffer.h"
#include "src/SerialTerminal/Terminal.h"
#include <string>
#include <sstream>

namespace Controller {

class TerminalController {
private:
  UnbufferedSerial *_pcPtr;
  Measurement::Buffer *_bufferPtr;

  void _transmit(string);
  void _writeToSerial(string);

public:
  TerminalController(UnbufferedSerial *, Measurement::Buffer *);
  ~TerminalController() = default;

  void attachHelp(SerialTerminal::Terminal *, string);
  void attachMemInfo(SerialTerminal::Terminal *, string);
  void attachTargetInfo(SerialTerminal::Terminal *, string);
  void attachBufferStatus(SerialTerminal::Terminal *, string);
  void attachBuffer(SerialTerminal::Terminal *, string);
  void attachBufferClear(SerialTerminal::Terminal *, string);
  void attachBufferShow(SerialTerminal::Terminal *, string);
};

} // namespace Controller

#endif