// (qt_lib)

#ifndef QT_LIB_WIN_HPP
#define QT_LIB_WIN_HPP

#include <dev_lib/inc/defs.hpp>
#include <QMainWindow>

namespace l_qt {
//------------------------------------------------------------------------------

struct split;
struct panel;

dcl_sub_(win, QMainWindow)
  win();

  mth_mut_(split&, hs, ())        may_err;
  mth_mut_(split&, vs, ())        may_err;
  mth_mut_(panel&, makePanel, ()) may_err;

  mth_mut_(void, show, (bool = true));

  virtual mth_mut_(void, onFirstShow, ());
  virtual mth_mut_(bool, onClose, ());

protected:
  void closeEvent(QCloseEvent*);
  bool firstShow;
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
