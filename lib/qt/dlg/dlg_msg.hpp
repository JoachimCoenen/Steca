// (qt_lib)

#pragma once
#include "../defs.hpp"

class QWidget;

namespace l_qt {
//------------------------------------------------------------------------------

void dlgInfo(QWidget*, strc);
void dlgWarn(QWidget*, strc);
bool dlgYes(QWidget*,  strc);

//------------------------------------------------------------------------------
}
// eof DOCS
