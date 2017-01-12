// c

#include "log.h"

namespace c { namespace qt {
//------------------------------------------------------------------------------

void log::info(QString const& msg) {
  if (handler)
    handler(msg, INFO);
}

void log::warn(QString const& msg) {
  if (handler)
    handler(msg, WARN);
}

void log::popup(QString const& msg) {
  if (handler)
    handler(msg, POPUP);
}

void (*log::handler)(QString const&, eType) = nullptr;

//------------------------------------------------------------------------------
}}
// eof
