// (qt_lib)

#ifndef QT_LIB_DLG_MSG_H
#define QT_LIB_DLG_MSG_H

#include <dev_lib/inc/defs_h.h>
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
