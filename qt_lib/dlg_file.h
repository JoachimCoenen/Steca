// (qt_lib)

#ifndef QT_LIB_DLG_FILE_H
#define QT_LIB_DLG_FILE_H

#include <dev_lib/inc/defs_h.h>
#include "str_inc.h"

class QAbstractProxyModel;
class QWidget;

namespace l_qt {
//------------------------------------------------------------------------------

str     dlgOpenFile(QWidget*,  strc caption, strc dir, strc filter, QAbstractProxyModel* = nullptr);
str_vec dlgOpenFiles(QWidget*, strc caption, strc dir, strc filter, QAbstractProxyModel* = nullptr);
str     dlgSaveFile(QWidget*,  strc caption, strc dir, strc filter);

//------------------------------------------------------------------------------
}
#endif
// eof
