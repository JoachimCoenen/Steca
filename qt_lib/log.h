// (qt_lib)

#ifndef QT_LIB_LOG_H
#define QT_LIB_LOG_H

#include <dev_lib/inc/defs_h.h>

namespace l_qt {
//------------------------------------------------------------------------------

namespace log {
  enum eType { INFO, WARN, POPUP };

  void info(strc);
  void warn(strc);
  void popup(strc);

  extern void (*handler)(strc, eType);
}

//------------------------------------------------------------------------------
}
#endif
// eof
