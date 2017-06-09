// (app_lib)

#ifndef APP_LIB_TYP_QT_LOG_H
#define APP_LIB_TYP_QT_LOG_H

#include "../inc/defs_h.h"

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
