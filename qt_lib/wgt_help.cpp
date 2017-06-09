// (app_lib)

#include "wgt_help.h"
#include "../inc/defs_cpp.h"
#include <QMessageBox>

namespace l_qt {
//------------------------------------------------------------------------------

uint mWidth(QWidget const& w) {
  return l::to_u(w.fontMetrics().width("m"));
}

void dlgInfo(QWidget* parent, strc text) {
  QMessageBox::information(parent, "", text);
}

void dlgWarn(QWidget* parent, strc text) {
  QMessageBox::warning(parent, "", text);
}

bool dlgYes(QWidget* parent, strc text) {
  return QMessageBox::Yes == QMessageBox::question(parent, "", text);
}

//------------------------------------------------------------------------------
}
// eof
