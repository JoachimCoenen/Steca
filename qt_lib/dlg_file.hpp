// (qt_lib)

#pragma once

#include <dev_lib/inc/defs.hpp>
#include "str_inc.hpp"

class QAbstractProxyModel;
class QWidget;

namespace l_qt {
//------------------------------------------------------------------------------

str     dlgOpenFile(QWidget*,  strc caption, strc dir, strc filter, QAbstractProxyModel* = nullptr);
str_vec dlgOpenFiles(QWidget*, strc caption, strc dir, strc filter, QAbstractProxyModel* = nullptr);
str     dlgSaveFile(QWidget*,  strc caption, strc dir, strc filter);

//------------------------------------------------------------------------------
}
// eof
