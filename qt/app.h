// c

#ifndef C_QT_APP_H
#define C_QT_APP_H

#include <c/app/app.i.h>

#if _c_cpp_

#include <QApplication>
#include <QStyleFactory>

namespace qt {
//------------------------------------------------------------------------------

struct app : c::app, QApplication { BASE(QApplication)
  app(int& argc, char* argv[]);
  int exec();
};

//------------------------------------------------------------------------------
}
#endif
#endif
// eof
