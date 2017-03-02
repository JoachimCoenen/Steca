// (c)

#ifndef C_CPP_EXC_HPP
#define C_CPP_EXC_HPP

#include "../c/lib/str.h"
#include <exception>

#define NS__ c
#define ST__ exc

_struct_sub (::std::exception)
  _atr(str,  msg)
  _atr(bool, silent)

  _dcl(exc(pcstr)         noexcept;)
  _dcl(pcstr what() const noexcept;)
_struct_sub_end

#define may_exc  noexcept(false)
#define will_exc noexcept(false)

_ns_dcl([[noreturn]] void err()             will_exc;)
_ns_dcl([[noreturn]] void err(pcstr)        will_exc;)
_ns_dcl([[noreturn]] void err(pcstr, pcstr) will_exc;)

#undef ST__
#undef NS__

#endif
// eof
