// (qt_lib)

#pragma once
#include "../defs.hpp"
#include <QDialog>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_reimpl_(DlgAbout, QDialog)
  DlgAbout(QWidget*);

  int exec();

protected:
  virtual mth_(str,      infoText, ()) = 0;
  virtual mth_(QLayout*, extra,    ()) RET_(nullptr)

  mut_(construct, ());
dcl_end

//------------------------------------------------------------------------------
}
// eof DOCS
