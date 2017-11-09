// (qt_lib)

#pragma once
#include <lib/dev/defs.hpp>

class QWidget;

namespace l_qt {
//------------------------------------------------------------------------------

void dlgInfo(QWidget*, strc);
void dlgWarn(QWidget*, strc);
bool dlgYes(QWidget*,  strc);

//------------------------------------------------------------------------------
}
// eof
