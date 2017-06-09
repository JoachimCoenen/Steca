// (app_lib)

#ifndef APP_LIB_TYP_QT_WIN_H
#define APP_LIB_TYP_QT_WIN_H

#include "../inc/defs_h.h"
#include <QMainWindow>

namespace l_qt {
//------------------------------------------------------------------------------

struct split;
struct panel;

dcl_sub_(win, QMainWindow)
  win();

  mth_mut_err_(split&, hs, ())
  mth_mut_err_(split&, vs, ())
  mth_mut_err_(panel&, makePanel, ())

  mth_mut_(void, show, (bool = true))

  vir_mth_mut_(void, onFirstShow, ())
  vir_mth_mut_(bool, onClose, ())

protected:
  void closeEvent(QCloseEvent*);
  bool firstShow;
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
