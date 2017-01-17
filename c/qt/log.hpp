// c

#ifndef C_QT_LOG_HPP
#define C_QT_LOG_HPP

#include "../cpp/app/app.i.h"
#include "def.hpp"
#include <QApplication>

namespace c_qt {
//------------------------------------------------------------------------------

struct log {
  enum eType { INFO, WARN, POPUP };
  static void info(qstrc);
  static void warn(qstrc);
  static void popup(qstrc);

  static void (*handler)(qstrc, eType);
};

//------------------------------------------------------------------------------
}
#endif
// eof
