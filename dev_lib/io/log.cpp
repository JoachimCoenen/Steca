// (dev_lib)

#include "log.hpp"
#include "../defs.inc"

#include "../typ/vec.hpp"
#include <iostream>

namespace l_io { namespace log {
//------------------------------------------------------------------------------

static void stdHandler(strc msg, eType type) {
  switch (type) {
  case INFO:
    std::cout << "INFO ";
    break;
  case WARN:
    std::cout << "WARN ";
    break;
  case MODAL:
    std::cout << "MSG: ";
    break;
  }

  std::cout << msg;

  if (MODAL == type)
    while ('\n' != std::getchar())
      ;
}

static void (*handler)(strc, eType) = stdHandler;

void info(strc msg) {
  if (handler)
    handler(msg, INFO);
}

void warn(strc msg) {
  if (handler)
    handler(msg, WARN);
}

void popup(strc msg) {
  if (handler)
    handler(msg, MODAL);
}

static l::vec<handler_t> handlers;

void set(handler_t handler_) {
  handlers.add(handler);
  handler = handler_;
}

void unset() {
  handler = handlers.pop();
}

//------------------------------------------------------------------------------
}}
// eof
