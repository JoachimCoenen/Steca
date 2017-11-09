// (qt_lib)

#include "dlg_file.hpp"
#include "../defs.inc"
#include <QFileDialog>
#include <QStyle>

namespace l_qt {
//------------------------------------------------------------------------------

l_io::path dlgOpenFile(QWidget* parent, strc caption, strc dir, strc filter, QAbstractProxyModel* model) {
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

  return str::null;
}

l_io::path_vec  dlgOpenFiles(QWidget* parent, strc caption, strc dir, strc filter, QAbstractProxyModel* model) {
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

l_io::path dlgSaveFile(QWidget* parent, strc caption, strc dir, strc filter, QAbstractProxyModel* model) {
  QFileDialog dlg(parent, toQt(caption), toQt(dir), toQt(filter));

  dlg.setOption(QFileDialog::DontUseNativeDialog);
  dlg.setViewMode(QFileDialog::Detail);
  dlg.setFileMode(QFileDialog::AnyFile);
  dlg.setAcceptMode(QFileDialog::AcceptSave);
  dlg.setConfirmOverwrite(false);

  if (model)
    dlg.setProxyModel(model);

  if (dlg.exec() && !dlg.selectedFiles().isEmpty())
    return fromQt(dlg.selectedFiles().first());

  return str::null;
}

l_io::path dlgSaveDir(QWidget* parent, strc caption, strc dir) {
  QFileDialog dlg(parent, toQt(caption), toQt(dir));

  dlg.setOption(QFileDialog::DontUseNativeDialog);
  dlg.setViewMode(QFileDialog::Detail);
  dlg.setFileMode(QFileDialog::Directory);
  dlg.setAcceptMode(QFileDialog::AcceptSave);
  dlg.setConfirmOverwrite(false);

  if (dlg.exec() && !dlg.selectedFiles().isEmpty())
    return fromQt(dlg.selectedFiles().first());

  return str::null;
}

//------------------------------------------------------------------------------
}
// eof
