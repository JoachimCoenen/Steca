// c

#ifndef C_CPP_EXC_H
#define C_CPP_EXC_H

#include <c/lib/str.h>
#include <exception>

_c_cpp_sub_struct(exc, ::std::exception)
  _c_var(str,  msg)
  _c_var(bool, silent)

  exc(strc)          noexcept;
  pcstr what() const noexcept;

  [[noreturn]] static void err(strc) noexcept(false);
_c_cpp_sub_struct_end

/* TODO
// An exception that carries a message.
class Exception : public QException {
  CLASS(Exception) SUPER(QException)
protected:
  Exception(rcstr msg, bool silent) noexcept;
public:

  Exception()           noexcept;
  Exception(rcstr msg)  noexcept;
  Exception(rc)         noexcept;

  void setMsg(rcstr);

  Exception* clone() const;
  void       raise() const;

protected:
  QByteArray msg8bit_;
};

// raise an exception
#define THROW(msg)     throw Exception(msg)
#define THROW_SILENT() throw Exception()

// run-time condition checking
#define RUNTIME_CHECK(test, msg) \
  if (!(test)) THROW(msg)
*/
//------------------------------------------------------------------------------
#endif
// eof
