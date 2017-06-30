// (qt_lib)

#include "dlg_file.hpp"
#include <dev_lib/inc/defs.inc>

#include <QFileDialog>

namespace l_qt {
//------------------------------------------------------------------------------

str dlgOpenFile(QWidget* parent, strc caption, strc dir, strc filter, QAbstractProxyModel* model) {
  QFileDialog dlg(parent, toQt(caption), toQt(dir), toQt(filter));

  dlg.setOption(QFileDialog::DontUseNativeDialog);
  dlg.setViewMode(QFileDialog::Detail);
  dlg.setFileMode(QFileDialog::ExistingFile);
  dlg.setAcceptMode(QFileDialog::AcceptOpen);
  dlg.setReadOnly(true);

  if (model)
    dlg.setProxyModel(model);

  if (dlg.exec() && !dlg.selectedFiles().isEmpty())
    return fromQt(dlg.selectedFiles().first());

  return nullstr;
}

str_vec dlgOpenFiles(QWidget* parent, strc caption, strc dir, strc filter, QAbstractProxyModel* model) {
  QFileDialog dlg(parent, toQt(caption), toQt(dir), toQt(filter));

  dlg.setOption(QFileDialog::DontUseNativeDialog);
  dlg.setViewMode(QFileDialog::Detail);
  dlg.setFileMode(QFileDialog::ExistingFiles);
  dlg.setAcceptMode(QFileDialog::AcceptOpen);
  dlg.setReadOnly(true);

  if (model)
    dlg.setProxyModel(model);

  if (dlg.exec())
    return fromQt(dlg.selectedFiles());

  return str_vec();
}

str dlgSaveFile(QWidget* parent, strc caption, strc dir, strc filter) {
  QFileDialog dlg(parent, toQt(caption), toQt(dir), toQt(filter));

  dlg.setOption(QFileDialog::DontUseNativeDialog);
  dlg.setViewMode(QFileDialog::Detail);
  dlg.setFileMode(QFileDialog::AnyFile);
  dlg.setAcceptMode(QFileDialog::AcceptSave);
  dlg.setConfirmOverwrite(false);

  if (dlg.exec() && !dlg.selectedFiles().isEmpty())
    return fromQt(dlg.selectedFiles().first());

  return nullstr;
}

//------------------------------------------------------------------------------
}
// eof
