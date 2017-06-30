// (qt_lib)

#pragma once

#include <dev_lib/inc/defs.hpp>
#include "win.hpp"

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
// eof
