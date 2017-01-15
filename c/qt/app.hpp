// c

#ifndef C_QT_APP_H
#define C_QT_APP_H

#include <c/cpp/app/app.i.h>
#include "win.hpp"
#include <QApplication>

namespace c_qt {
//------------------------------------------------------------------------------

struct app : c::app, QApplication { using base = QApplication;
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
}
#endif
// eof
