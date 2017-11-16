// (lib/qt)

/** @file
 * a standard about dialog
 */

#pragma once

#include "../defs.hpp"
#include <QDialog>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_reimpl_(DlgAbout, QDialog)
  DlgAbout(QWidget*);

  int exec();

protected:
  /// provide info text
  virtual mth_(str,      infoText, ()) = 0;
  /// optionally provide a layout with extra content
  virtual mth_(QLayout*, extra,    ()) RET_(nullptr)

  mut_(construct, ());
dcl_end

//------------------------------------------------------------------------------
}
// eof
