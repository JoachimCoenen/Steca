// (dev_lib)

#include "log.hpp"
#include "../defs.inc"

#include "../typ/vec.hpp"
#include <iostream>

namespace l_io {
//------------------------------------------------------------------------------

static void stdHandler(strc msg, log::eType type) {
  switch (type) {
  case log::INFO:
    std::cout << "INFO ";
    break;
  case log::WARN:
    std::cout << "WARN ";
    break;
  case log::MODAL:
    std::cout << "MSG: ";
    break;
  }

  std::cout << msg;

  if (log::MODAL == type)
    while ('\n' != std::getchar())
      ;
}

static void (*logHandler)(strc, log::eType) = stdHandler;

void log::info(strc msg) {
  if (logHandler)
    logHandler(msg, INFO);
}

void log::warn(strc msg) {
  if (logHandler)
    logHandler(msg, WARN);
}

void log::modal(strc msg) {
  if (logHandler)
    logHandler(msg, MODAL);
}

static l::vec<log::handler_t> logHandlers;

void log::set(handler_t handler) {
  logHandlers.add((logHandler = handler));
}

void log::unset() {
  logHandler = logHandlers.pop();
}

//------------------------------------------------------------------------------

static void (*busyHandler)(bool) = nullptr;

static l::vec<busy::handler_t> busyHandlers;

void busy::set(handler_t handler) {
  busyHandlers.add((busyHandler = handler));
}

void busy::unset() {
  busyHandler = busyHandlers.pop();
}

busy::busy() {
  if (busyHandler)
    busyHandler(true);
}

busy::~busy() {
  if (busyHandler)
    busyHandler(false);
}

//------------------------------------------------------------------------------
}
// eof
