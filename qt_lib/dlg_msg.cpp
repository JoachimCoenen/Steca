// (qt_lib)

#include "dlg_msg.hpp"
#include <dev_lib/defs.inc>

#include "str_inc.hpp"

#include <QMessageBox>

namespace l_qt {
//------------------------------------------------------------------------------

void dlgInfo(QWidget* parent, strc text) {
  QMessageBox::information(parent, "", toQt(text));
}

void dlgWarn(QWidget* parent, strc text) {
  QMessageBox::warning(parent, "", toQt(text));
}

bool dlgYes(QWidget* parent, strc text) {
  return QMessageBox::Yes == QMessageBox::question(parent, "", toQt(text));
}

//------------------------------------------------------------------------------
}
// eof
