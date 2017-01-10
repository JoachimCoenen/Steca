// c

#ifndef C_QT_WIN_H
#define C_QT_WIN_H

#include <c/app/win.i.h>

#if __cpp__

#include <QMainWindow>

namespace qt {
//------------------------------------------------------------------------------

struct win : c::win, QMainWindow { BASE(QMainWindow)
  win();
  void show();
};

//------------------------------------------------------------------------------
}
#endif
#endif
// eof
