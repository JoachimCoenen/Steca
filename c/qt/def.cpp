// c

#include "def.hpp"

namespace c { namespace qt {
//------------------------------------------------------------------------------

[[noreturn]] void err(qstrc msg) will_exc {
  c::err(msg.toLocal8Bit());
}

//------------------------------------------------------------------------------
}}
// eof
