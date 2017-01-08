// c

#ifndef C_QT_APP_H
#define C_QT_APP_H

#include "../../h"

#if __cpp__

#include <QApplication>
#include <QStyleFactory>

namespace c { namespace qt {
//------------------------------------------------------------------------------

struct app : QApplication { BASE(QApplication)
  app(int& argc, char* argv[]);
  int exec();
};

//------------------------------------------------------------------------------
}}
#endif
#endif
// eof
