// c

#ifndef C_CPP_EXC_H
#define C_CPP_EXC_H

#include <c/c/lib/str.h>
#include <exception>

#define DATA_NS    c
#define DATA_NAME  exc

_struct_sub(::std::exception)
  _var (str,  msg)
  _var (bool, silent)

  _extra(exc(pcstr)         noexcept;)
  _extra(pcstr what() const noexcept;)
_struct_sub_end

#define may_exc  noexcept(false)
#define will_exc noexcept(false)

_ns_extra([[noreturn]] void err(pcstr) will_exc;)

#undef DATA_NS
#undef DATA_NAME

#endif
// eof
