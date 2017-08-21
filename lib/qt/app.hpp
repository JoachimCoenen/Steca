// (qt_lib)

#pragma once
#include <lib/dev/defs.hpp>
#include <QApplication>

namespace l_qt {
//------------------------------------------------------------------------------

struct win;

dcl_sub_(app, QApplication)
  app(int& argc, char* argv[]);

  int exec(win* = nullptr);
  int safeExec(win* = nullptr);

  dcl_(NoWarnings)
    NoWarnings();
   ~NoWarnings();
  dcl_end

private:
  bool notify(QObject*, QEvent*);
dcl_end

//------------------------------------------------------------------------------
}
// eof
