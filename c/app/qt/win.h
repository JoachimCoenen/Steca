// c

#ifndef C_QT_WIN_H
#define C_QT_WIN_H

#include "../../h"

#if __cpp__

#include <QMainWindow>

namespace c { namespace qt {
//------------------------------------------------------------------------------

struct win : QMainWindow { BASE(QMainWindow)
  win();
};

//------------------------------------------------------------------------------
}}
#endif
#endif
// eof
