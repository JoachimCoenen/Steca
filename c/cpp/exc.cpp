// c

#include "exc.h"
#include <c/c/cpp>

namespace c {
//------------------------------------------------------------------------------

exc::exc(strc msg_) noexcept : base(), msg(msg_), silent(false) {
}

pcstr exc::what() const noexcept {
  return msg;
}

void exc::err(strc msg) {
  throw exc(msg);
}

TEST("exc", ({
  CHECK_THROWS_AS(exc::err(""), exc&);
});)

/*
TODO
#ifdef TESTS
#include "def/def_gsl.h"
#endif

Exception::Exception(rcstr msg, bool silent) noexcept
: silent_(silent) {
  setMsg(msg);
}


Exception::Exception() noexcept
: Cls(EMPTY_STR, true) {
}

TEST("Exception()",
  CHECK_THROWS_AS(THROW("Hello"), Exception);
  CHECK_THROWS_AS(THROW_SILENT(), Exception);

  CHECK_NOTHROW(Exception());
  CHECK_THROWS_AS(Exception().raise(), Exception);

  scoped<Exception*> p(Exception().clone());
  CHECK_THROWS_AS(p->raise(), Exception);
)

Exception::Exception(rcstr msg) noexcept
: Cls(msg, false) {
}

TEST("Exception(\"Hi\")",
  try {
    THROW("Hi");
  } catch (Exception &e) {
    CHECK_EQ("Hi", e.msg());
    return;
  }

  CHECK(false); // not here
)

Exception::Exception(rc that) noexcept
: Cls(that.msg_) {
}


TEST("Exception(\"Hi\")",
  try {
    THROW("Hi");
  } catch (Exception &e) {
    CHECK_EQ("Hi", str(e.what()));
    return;
  }

  CHECK(false); // not here
)

void Exception::setMsg(rcstr s) {
  msg_     = s;
  msg8bit_ = msg_.toLocal8Bit();
}

Exception* Exception::clone() const {
  return new Exception(*this);
}

void Exception::raise() const {
  throw *this;
}
*/
//------------------------------------------------------------------------------
}
// eof
