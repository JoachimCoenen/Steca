// (app_lib)

#ifndef APP_LIB_TYP_QT_DLG_FILE_HPP
#define APP_LIB_TYP_QT_DLG_FILE_HPP

#include "../inc/defs_h.h"
#include <QWidget>

class QAbstractProxyModel;

namespace l_qt {
//------------------------------------------------------------------------------

str     dlgOpenFile(QWidget*,  strc caption, strc dir, strc filter, QAbstractProxyModel* = nullptr);
str_lst dlgOpenFiles(QWidget*, strc caption, strc dir, strc filter, QAbstractProxyModel* = nullptr);
str     dlgSaveFile(QWidget*,  strc caption, strc dir, strc filter);

//------------------------------------------------------------------------------
}
#endif
// eof
