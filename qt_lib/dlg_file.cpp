// (app_libc::str)

#include "dlg_file.h"
#include "../inc/defs_cpp.h"
#include <QFileDialog>
//#include <QMessageBox>

namespace l_qt {
//------------------------------------------------------------------------------

str dlgOpenFile(QWidget* parent, strc caption, strc dir, strc filter, QAbstractProxyModel* model) {
  QFileDialog dlg(parent, caption, dir, filter);

  dlg.setOption(QFileDialog::DontUseNativeDialog);
  dlg.setViewMode(QFileDialog::Detail);
  dlg.setFileMode(QFileDialog::ExistingFile);
  dlg.setAcceptMode(QFileDialog::AcceptOpen);
  dlg.setReadOnly(true);

  if (model)
    dlg.setProxyModel(model);

  if (dlg.exec() && !dlg.selectedFiles().isEmpty())
    return dlg.selectedFiles().first();

  return str::null;
}

str_lst dlgOpenFiles(QWidget* parent, strc caption, strc dir, strc filter, QAbstractProxyModel* model) {
  QFileDialog dlg(parent, caption, dir, filter);

  dlg.setOption(QFileDialog::DontUseNativeDialog);
  dlg.setViewMode(QFileDialog::Detail);
  dlg.setFileMode(QFileDialog::ExistingFiles);
  dlg.setAcceptMode(QFileDialog::AcceptOpen);
  dlg.setReadOnly(true);

  if (model)
    dlg.setProxyModel(model);

  if (dlg.exec())
    return dlg.selectedFiles();

  return str_lst();
}

str dlgSaveFile(QWidget* parent, strc caption, strc dir, strc filter) {
  QFileDialog dlg(parent, caption, dir, filter);

  dlg.setOption(QFileDialog::DontUseNativeDialog);
  dlg.setViewMode(QFileDialog::Detail);
  dlg.setFileMode(QFileDialog::AnyFile);
  dlg.setAcceptMode(QFileDialog::AcceptSave);
  dlg.setConfirmOverwrite(false);

  if (dlg.exec() && !dlg.selectedFiles().isEmpty())
    return dlg.selectedFiles().first();

  return str::null;
}

//------------------------------------------------------------------------------
}
// eof
