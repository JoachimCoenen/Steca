// c

#include "def.hpp"
#include <QString>

c::str toStr(qstrc qs) {
  return c::str(pcstr(qs.toLocal8Bit()));
}

namespace c_qt {
//------------------------------------------------------------------------------

[[noreturn]] void err(qstrc msg) will_exc {
  c::err(msg.toLocal8Bit());
}

//------------------------------------------------------------------------------
}
// eof
