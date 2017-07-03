// (dev_lib)

#include "exc.hpp"
#include "../defs.inc"

namespace l {
//------------------------------------------------------------------------------

exc::exc(strc msg_) noexcept : msg(msg_), silent(false) {}

[[noreturn]] void err() will_err {
  throw exc(nullstr);
}

[[noreturn]] void err(strc msg) will_err {
  throw exc(msg);
}

TEST_("exc",
  CHECK_THROWS_AS(err(""), exc&);
  bool caught = false;
  try {
    err("Hello");
  } catch (exc const& e) {
    caught = true;
    CHECK_EQ(str("Hello"), e.msg);
  }

  CHECK(caught);
)

//------------------------------------------------------------------------------
}
// eof
