// (qt_lib)

#include "win.h"
#include <dev_lib/inc/defs_cpp.h>

#include "layout.h"
#include "split.h"
#include <QCloseEvent>

namespace l_qt {
//------------------------------------------------------------------------------

win::win() : firstShow(true) {}

static split& _hvs(split* s, win* w) {
  EXPECT_(s) EXPECT_(w)
  check_or_err_(w->centralWidget(), "already have a central widget");
  w->setCentralWidget(s);
  return *s;
}

split& win::hs() may_err {
  return _hvs(new hSplit, this);
}

split& win::vs() may_err {
  return _hvs(new vSplit, this);
}

panel& win::makePanel() may_err {
  check_or_err_(!base::centralWidget(), "already have a central widget");
  auto p = new panel;
  base::setCentralWidget(p);
  return *p;
}

void win::show(bool on) {
  if (on && firstShow) {
    firstShow = false;
    onFirstShow();
  }

  base::setVisible(on);
}

void win::onFirstShow() {}
bool win::onClose()     VAL_(true)

void win::closeEvent(QCloseEvent* e) {
  if (onClose())
    e->accept();
  else
    e->ignore();
}

//------------------------------------------------------------------------------
}
// eof
