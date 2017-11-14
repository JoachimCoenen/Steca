// (qt_lib)

#pragma once
#include "../defs.hpp"
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

  virtual mut_(show,  (bool = true));
  virtual mut_(fullScreen, (bool));
  virtual mut_(about, ()) {}
  virtual mut_(quit,  ());

  virtual mut_(onFirstShow, ()) {}
  virtual bol_mut_(onClose,     ()) RET_(true)

protected:
  void closeEvent(QCloseEvent*);
  bool firstShow;
  QByteArray initialState;
dcl_end

//------------------------------------------------------------------------------
}
// eof DOCS
