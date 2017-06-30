// (qt_lib)

#ifndef QT_LIB_DLG_MSG_HPP
#define QT_LIB_DLG_MSG_HPP

#include <dev_lib/inc/defs.hpp>
#include <QWidget>

namespace l_qt {
//------------------------------------------------------------------------------

void dlgInfo(QWidget*, strc);
void dlgWarn(QWidget*, strc);
bool dlgYes(QWidget*,  strc);

//------------------------------------------------------------------------------
}
#endif
// eof
