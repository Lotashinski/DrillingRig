#include "mbed.h"
#include "mbed_mem_trace.h"

#include "src/Controller/TerminalController.h"
#include "src/Measurement/Buffer.h"
#include "src/SerialTerminal/Terminal.h"

#include <bitset>
#include <cstdint>
#include <cstdio>
#include <ios>
#include <ostream>
#include <sstream>
#include <string>

#define AS_LABDA(func) [](Terminal *term, string data) { func(term, data); }

using std::string;
using namespace SerialTerminal;

static Thread t;
static EventQueue eq(EVENTS_EVENT_SIZE * 16);

static InterruptIn gercon(BUTTON1);
static UnbufferedSerial bt(PB_6, PA_10, 9600);

UnbufferedSerial &get_serial();

static Terminal term;
static Measurement::Buffer mBuffer(1024);
static Controller::TerminalController terminaController(&get_serial(),
                                                        &mBuffer);

void on_rx_interrupt();
void attached_callbacks(Terminal &);
void gercon_rice_interput();

void write_on_terminal(char);

int main() {
  t.start(callback(&eq, &EventQueue::dispatch_forever));

  get_serial().attach(&on_rx_interrupt, SerialBase::RxIrq);
  gercon.rise(&gercon_rice_interput);
  attached_callbacks(term);
}

UnbufferedSerial &get_serial() { return bt; }

void on_rx_interrupt() {
  char c;
  auto &us = get_serial();

  if (us.read(&c, 1))
    write_on_terminal(c);
}

void write_on_terminal(char c) {
  eq.call([c]() { term.write(c); });
}

void attached_callbacks(Terminal &term) {
  term.attach("HELP", AS_LABDA(terminaController.attachHelp));
  term.attach("MEM_INFO", AS_LABDA(terminaController.attachMemInfo));
  term.attach("TARGET_INFO", AS_LABDA(terminaController.attachTargetInfo));
  term.attach("BUFFER_STATUS", AS_LABDA(terminaController.attachBufferStatus));
  term.attach("BUFFER", AS_LABDA(terminaController.attachBuffer));
  term.attach("BUFFER_CLEAR", AS_LABDA(terminaController.attachBufferClear));
  term.attach("BUFFER_SHOW", AS_LABDA(terminaController.attachBufferShow));
}

void gercon_rice_interput() {
  auto currentIndex = mBuffer.getCurrentIndex();
  mBuffer.addMeasurement(2.1 * currentIndex);
}