// (app_lib)

#ifndef APP_LIB_TYP_QT_APP_H
#define APP_LIB_TYP_QT_APP_H

#include "../inc/defs_h.h"
#include "win.h"
#include <QApplication>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(app, QApplication)
  app(int& argc, char* argv[]);

  int exec();
  int execWin(win&);

  template <typename Win>
  static int run(int& argc, char* argv[]) {
    app a(argc, argv);
    Win w;
    return a.runWin(w);
  }

private:
  int runWin(win&);
  int execWin(win*);
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
