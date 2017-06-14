// (qt_lib)

#ifndef QT_LIB_APP_H
#define QT_LIB_APP_H

#include <dev_lib/inc/defs_h.h>
#include "win.h"

#include <QApplication>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(app, QApplication)
  app(int& argc, char* argv[]);

  int exec(win* = nullptr);
  int safeExec(win* = nullptr);

private:
  bool notify(QObject*, QEvent*);
dcl_end

//------------------------------------------------------------------------------

struct busy_indicator final {
public:
  busy_indicator();
 ~busy_indicator();

  static void (*handler)(bool);
};

//------------------------------------------------------------------------------
}
#endif
// eof