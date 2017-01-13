// c

#ifndef C_QT_LOG_H
#define C_QT_LOG_H

#include <c/cpp/app/app.i.h>
#include <c/qt/def.h>
#include <QApplication>

namespace c { namespace qt {
//------------------------------------------------------------------------------

struct log {
  enum eType { INFO, WARN, POPUP };
  static void info(qstrc);
  static void warn(qstrc);
  static void popup(qstrc);

  static void (*handler)(qstrc, eType);
};

//------------------------------------------------------------------------------
}}
#endif
// eof
