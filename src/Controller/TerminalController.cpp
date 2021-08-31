#include "TerminalController.h"

#define STR(v) #v
#define STRINGFY(v) STR(v)

#define AS_LAMBDA(func)                                                        \
  [this](SerialTerminal::Terminal *term, string data) { func(term, data); }

using namespace Controller;

TerminalController::TerminalController(UnbufferedSerial *pcPtr,
                                       Measurement::Buffer *buffer)
    : _pcPtr(pcPtr), _bufferPtr(buffer) {}

void TerminalController::attachHelp(SerialTerminal::Terminal *terminal,
                                     string) {
  auto events = terminal->getEvents();
  stringstream strStream;

  for (auto const &event : events)
    strStream << ">" << event << "\n";

  _transmit(strStream.str());
}

void TerminalController::attachMemInfo(SerialTerminal::Terminal *, string) {
  int cnt = osThreadGetCount();
  mbed_stats_stack_t *stats =
      (mbed_stats_stack_t *)malloc(cnt * sizeof(mbed_stats_stack_t));

  stringstream strFormater;

  cnt = mbed_stats_stack_get_each(stats, cnt);
  for (int i = 0; i < cnt; i++) {
    strFormater.setf(ios::left);
    strFormater << "Thread: 0x" << std::hex << stats[i].thread_id
                << ", Stack size:";

    strFormater.setf(ios::right);
    strFormater.width(8);
    strFormater << std::dec << stats[i].max_size << " /";

    strFormater.setf(ios::right);
    strFormater.width(8);
    strFormater << stats[i].reserved_size << " bytes\n";
  }
  free(stats);

  // Grab the heap statistics
  mbed_stats_heap_t heap_stats;
  mbed_stats_heap_get(&heap_stats);

  strFormater << "Heap size: ";

  strFormater.setf(ios::right);
  strFormater.width(8);
  strFormater << heap_stats.current_size << " /";

  strFormater.setf(ios::right);
  strFormater.width(8);
  strFormater << heap_stats.reserved_size << " bytes\n";

  _transmit(strFormater.str());
}

void TerminalController::attachTargetInfo(SerialTerminal::Terminal *, string) {
  stringstream strFormater;
  strFormater << "Mbed OS (" << MBED_MAJOR_VERSION << "." << MBED_MINOR_VERSION
              << "." << MBED_PATCH_VERSION << ") ";
  strFormater << STRINGFY(TARGET_NAME) << "\n";

  _transmit(strFormater.str());
}

void TerminalController::attachBufferStatus(SerialTerminal::Terminal *,
                                             string) {
  stringstream strFormater;

  strFormater.setf(ios::right);
  strFormater.width(8);
  strFormater << _bufferPtr->getCurrentIndex() << " /";

  strFormater.setf(ios::right);
  strFormater.width(8);
  strFormater << _bufferPtr->getBufferSize() << "\n";

  _transmit(strFormater.str());
}

void TerminalController::attachBuffer(SerialTerminal::Terminal *,
                                       string data) {
  auto delim = " ";
  auto pos = data.find(delim);
  pos = (pos == string::npos) ? data.length() : pos;

  auto currentIndex = _bufferPtr->getCurrentIndex();

  auto flag = data.substr(pos, data.length());
  auto index = 0;

  stringstream ss;
  ss << flag;
  ss >> index;

  if (index == 0)
    index = (currentIndex) ? currentIndex - 1 : 0;

  if (index < 0) {
    _transmit("ERR OUT_OF_RANGE\n");
  } else if (index >= currentIndex) {
    _transmit(to_string(index) +":UNDEFINED\n");
  } else {
    auto data = _bufferPtr->getMeasurement(index);
    auto out = to_string(index) + ":" + to_string(data) + "\n";
    _transmit(out);
  }
}

void TerminalController::attachBufferClear(SerialTerminal::Terminal *,
                                            string) {
  _bufferPtr->clearBuffer();
  _transmit("CLEAR\n");
}

void TerminalController::attachBufferShow(SerialTerminal::Terminal *, string) {
  stringstream strFormater;
  auto currentIndex = _bufferPtr->getCurrentIndex();

  for (auto i = 0; i < currentIndex; ++i) {
    strFormater.setf(ios::right);
    strFormater.width(8);
    strFormater << i << ": ";

    strFormater.setf(ios::right);
    strFormater.width(8);
    strFormater << _bufferPtr->getMeasurement(i);

    strFormater << "\n";
  }

  _transmit(strFormater.str());
}

void TerminalController::_transmit(string data) { _writeToSerial(data + "\r"); }

void TerminalController::_writeToSerial(string data) {
  _pcPtr->write(data.c_str(), data.length());
}
