// cpp

#ifndef CPP_EXC_H
#define CPP_EXC_H

#include <c/lib/str.h>
#include <exception>

struct exc : std::exception { BASE(std::exception)
  c::str msg;
  exc(strc);

  [[noreturn]] static void err(strc) noexcept(false);
};

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

  bool  silent() const noexcept { return silent_; }
  rcstr msg()    const noexcept { return msg_;    }
  pcstr what()   const noexcept;

  void setMsg(rcstr);

  Exception* clone() const;
  void       raise() const;

protected:
  str        msg_;
  QByteArray msg8bit_;
  bool       silent_;
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
