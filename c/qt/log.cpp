// c

#include "log.hpp"

namespace c { namespace qt {
//------------------------------------------------------------------------------

void log::info(qstrc msg) {
  if (handler)
    handler(msg, INFO);
}

void log::warn(qstrc msg) {
  if (handler)
    handler(msg, WARN);
}

void log::popup(qstrc msg) {
  if (handler)
    handler(msg, POPUP);
}

void (*log::handler)(qstrc, eType) = nullptr;

//------------------------------------------------------------------------------
}}
// eof
