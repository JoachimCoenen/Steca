// (qt_lib)

#include "win.hpp"
#include "defs.inc"
#include "act.hpp"
#include "settings.hpp"
#include "layout.hpp"
#include "split.hpp"
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
  base::setVisible(on);
  if (on && firstShow) {
    firstShow = false;

    if (initialState.isEmpty())
      initialState = saveState();

    using S = Settings;
    S s(S::GROUP_MAINWIN);
    restoreGeometry(s.value(S::GEOMETRY).toByteArray());
    restoreState(s.value(S::STATE).toByteArray());

    onFirstShow();
  }
}

void win::fullScreen(bool on) {
  if (on)
    showFullScreen();
  else
    showNormal();

#ifndef Q_OS_OSX
  auto&& as = getActs();
  as.get(as.FULL_SCREEN).check(on);
#endif
}

void win::quit() {
  base::close();
}

void win::closeEvent(QCloseEvent* e) {
  if (onClose()) {
    using S = Settings;
    S s(S::GROUP_MAINWIN);
    s.setValue(S::GEOMETRY, saveGeometry());
    s.setValue(S::STATE, saveState());

    e->accept();
  } else {
    e->ignore();
  }
}

//------------------------------------------------------------------------------
}
// eof
