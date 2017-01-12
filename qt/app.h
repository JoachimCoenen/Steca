// c

#ifndef C_QT_APP_H
#define C_QT_APP_H

#include <c/app/app.i.h>
#include "win.h"
#include <QApplication>

namespace c { namespace qt {
//------------------------------------------------------------------------------

struct app : c::app, QApplication { BASE(QApplication)
  app(int& argc, char* argv[]);
  int exec(win*);

private:
  bool notify(QObject*, QEvent*);
};

//------------------------------------------------------------------------------

struct busy_indicator final {
public:
  busy_indicator();
 ~busy_indicator();

  static void (*handler)(bool);
};

//------------------------------------------------------------------------------
}}
#endif
// eof
