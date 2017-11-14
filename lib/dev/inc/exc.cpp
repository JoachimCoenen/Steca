// (dev_lib)

#include "exc.hpp"
#include "defs.inc"

namespace l {
//------------------------------------------------------------------------------

exc::exc(strc msg_, bool silent_) noexcept : msg(msg_), silent(silent_) {}

[[noreturn]] void err() will_err {
  throw exc(str::null, false);
}

[[noreturn]] void err(strc msg) will_err {
  throw exc(msg, false);
}

[[noreturn]] void err(pcstr msg) will_err {
  throw exc(msg, false);
}

[[noreturn]] void errSilent() will_err {
  throw exc(str::null, true);
}

[[noreturn]] void errSilent(strc msg) will_err {
  throw exc(msg, true);
}

[[noreturn]] void errSilent(pcstr msg) will_err {
  throw exc(msg, true);
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
// eof DOCS
