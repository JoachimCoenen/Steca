// (qt_lib)

#pragma once
#include <lib/dev/defs.hpp>
#include <QMainWindow>

namespace l_qt {
//------------------------------------------------------------------------------

struct acts;
struct split;
struct panel;

dcl_sub_(win, QMainWindow)
  win();

  mth_mut_(split&, hs, ())        may_err;
  mth_mut_(split&, vs, ())        may_err;
  mth_mut_(panel&, makePanel, ()) may_err;

  virtual mth_(acts const&, getActs, ()) = 0;

  virtual act_mut_(show,  (bool = true));
  virtual act_mut_(fullScreen, (bool));
  virtual act_mut_(about, ()) {}
  virtual act_mut_(quit,  ());

  virtual act_mut_(onFirstShow, ()) {}
  virtual bol_mut_(onClose,     ()) RET_(true)

protected:
  void closeEvent(QCloseEvent*);
  bool firstShow;
  QByteArray initialState;
dcl_end

//------------------------------------------------------------------------------
}
// eof
