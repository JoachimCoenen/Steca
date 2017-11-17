// (lib/qt)

#include "win.hpp"
#include "../defs.inc"
#include "../app/act.hpp"
#include "../app/settings.hpp"
#include "../win/layout.hpp"
#include "../win/split.hpp"
#include <QCloseEvent>

namespace l_qt {
//------------------------------------------------------------------------------

win::win() : firstShow(true) {}

static split& _hvs(split* s, win* w) {
  EXPECT_(s) EXPECT_(w) EXPECT_(!w->centralWidget())
  w->setCentralWidget(s);
  return *s;
}

split& win::hs() {
  return _hvs(new hSplit, this);
}

split& win::vs() {
  return _hvs(new vSplit, this);
}

panel& win::makePanel() {
  EXPECT_(!base::centralWidget())
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
