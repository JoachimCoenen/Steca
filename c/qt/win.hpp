// (c)

#ifndef C_QT_WIN_HPP
#define C_QT_WIN_HPP

#include "../cpp/app/win.i.h"
#include <QMainWindow>

namespace c_qt {
//------------------------------------------------------------------------------

struct win : c::win, QMainWindow { using base = QMainWindow;
  win();
  void show();
};

//------------------------------------------------------------------------------
}
#endif
// eof
