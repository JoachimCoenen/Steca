// (c)

#include "def.hpp"
#include <QString>

c::str toStr(qstrc qs) {
  return c::str(pcstr(qs.toLocal8Bit()));
}

// eof
