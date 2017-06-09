// (app_lib)

#ifndef APP_LIB_TYP_QT_WGT_HELP_HPP
#define APP_LIB_TYP_QT_WGT_HELP_HPP

#include "../inc/defs_h.h"
#include <QWidget>

namespace l_qt {
//------------------------------------------------------------------------------

uint mWidth(QWidget const&);

void dlgInfo(QWidget*, strc);
void dlgWarn(QWidget*, strc);
bool dlgYes(QWidget*,  strc);

//------------------------------------------------------------------------------
}
#endif
// eof
