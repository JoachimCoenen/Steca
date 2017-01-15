// c

#ifndef C_QT_WIN_H
#define C_QT_WIN_H

#include <c/cpp/app/win.i.h>
#include <QMainWindow>

namespace c { namespace qt {
//------------------------------------------------------------------------------

struct win : c::win, QMainWindow { using base = QMainWindow;
  win();
  void show();
};

//------------------------------------------------------------------------------
}}
#endif
// eof
