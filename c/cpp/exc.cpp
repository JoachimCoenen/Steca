// (c)

#include "exc.hpp"
#include "../c/cpp"

namespace c {
//------------------------------------------------------------------------------

exc::exc(pcstr msg_) noexcept : base(), msg(msg_), silent(false) {
}

pcstr exc::what() const noexcept {
  return msg;
}

[[noreturn]] void err() will_exc {
  throw exc(str::nul);
}

[[noreturn]] void err(pcstr msg) will_exc {
  throw exc(msg);
}

[[noreturn]] void err(pcstr msg, pcstr s) will_exc {
  throw exc(str::cat(msg, s));
}

TEST("exc",
  CHECK_THROWS_AS(err(""), exc&);
  bool caught = false;
  try {
    err("Hello");
  } catch (std::exception const& e) {
    caught = true;
    CHECK_EQ(str("Hello"), str(e.what()));
  }

  CHECK(caught);
)

//------------------------------------------------------------------------------
}
// eof
