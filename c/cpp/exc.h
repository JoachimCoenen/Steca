// c

#ifndef C_CPP_EXC_H
#define C_CPP_EXC_H

#include <c/c/lib/str.h>
#include <exception>

#define may_exc  noexcept(false)
#define will_exc noexcept(false)

_nc_sub_struct (exc, ::std::exception)
  _var (str,  msg)
  _var (bool, silent)

  exc(strc)          noexcept;
  pcstr what() const noexcept;

  [[noreturn]] static void err(strc) will_exc;
_nc_sub_struct_end

/* TODO
// An exception that carries a message.
class Exception : public QException {
  CLASS(Exception) SUPER(QException)
protected:
  Exception(qstrc msg, bool silent) noexcept;
public:

  Exception()           noexcept;
  Exception(qstrc msg)  noexcept;
  Exception(rc)         noexcept;

  void setMsg(qstrc);

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
