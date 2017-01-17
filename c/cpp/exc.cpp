// c

#include "exc.hpp"
#include "../c/cpp"

namespace c {
//------------------------------------------------------------------------------

exc::exc(pcstr msg_) noexcept : base(), msg(msg_), silent(false) {
}

pcstr exc::what() const noexcept {
  return msg;
}

[[noreturn]] void err(pcstr msg) will_exc {
  throw exc(msg);
}

TEST("exc",
  CHECK_THROWS_AS(err(""), exc&);
)

//------------------------------------------------------------------------------
}
// eof
