// (lib/qt)

/** @file
 * a set of standard file dialogs
 */

#pragma once

#include "../defs.hpp"
#include <lib/dev/io/path.hpp>

class QAbstractProxyModel;
class QWidget;

namespace l_qt {
//------------------------------------------------------------------------------

l_io::path     dlgOpenFile(QWidget*,  strc caption, strc dir, strc filter, QAbstractProxyModel* = nullptr);
l_io::path_vec dlgOpenFiles(QWidget*, strc caption, strc dir, strc filter, QAbstractProxyModel* = nullptr);
l_io::path     dlgSaveFile(QWidget*,  strc caption, strc dir, strc filter, QAbstractProxyModel* = nullptr);
l_io::path     dlgSaveDir(QWidget*,  strc caption, strc dir);

//------------------------------------------------------------------------------
}
// eof
