// (app_libv)

#include "log.h"

namespace l_qt { namespace log {
//------------------------------------------------------------------------------

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
    handler(msg, POPUP);
}

void (*handler)(strc, eType) = nullptr;

//------------------------------------------------------------------------------
}}
// eof
