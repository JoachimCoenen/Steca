// (qt_lib)

#include "dlg_msg.hpp"
#include "../defs.inc"
#include <QMessageBox>

namespace l_qt {
//------------------------------------------------------------------------------

void dlgInfo(QWidget* parent, strc text) {
  QMessageBox::information(parent, QString::null, toQt(text));
}

void dlgWarn(QWidget* parent, strc text) {
  QMessageBox::warning(parent, QString::null, toQt(text));
}

bool dlgYes(QWidget* parent, strc text) {
  return QMessageBox::Yes == QMessageBox::question(parent, QString::null, toQt(text));
}

//------------------------------------------------------------------------------
}
// eof DOCS
