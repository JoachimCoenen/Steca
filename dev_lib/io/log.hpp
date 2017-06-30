// (dev_lib)

#ifndef DEV_LIB_LOG_HPP
#define DEV_LIB_LOG_HPP

#include <dev_lib/inc/defs.hpp>

namespace l {
//------------------------------------------------------------------------------

namespace log {
  enum eType { INFO, WARN, MODAL };

  void info(strc);
  void warn(strc);
  void modal(strc);

  typedef void (*handler_t)(strc, eType);

  void set(handler_t);
  void unset();
}

//------------------------------------------------------------------------------
}
#endif
// eof
