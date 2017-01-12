// c

#ifndef C_QT_LOG_H
#define C_QT_LOG_H

#include <c/app/app.i.h>
#include <QApplication>

namespace c { namespace qt {
//------------------------------------------------------------------------------

struct log {
  enum eType { INFO, WARN, POPUP };
  static void info(QString const&);
  static void warn(QString const&);
  static void popup(QString const&);

  static void (*handler)(QString const&, eType);
};

//------------------------------------------------------------------------------
}}
#endif
// eof
