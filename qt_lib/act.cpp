// (qt_lib)

#include "act.h"
#include <dev_lib/inc/defs_cpp.h>
#include "str_inc.h"
#include "win.h"
#include <QMenuBar>

namespace l_qt {
//------------------------------------------------------------------------------

act::act(strc tx) : base(nullptr) {
  text(tx); tip(tx);
}

l::own<act> act::make(strc tx, strc key) {
  auto a = new act(tx); a->key(key);
  return l::owned(a);
}

l::own<act> act::make(strc tx, strc key, strc ic) {
  auto a = make(tx, key); mutp(a)->icon(ic);
  return a;
}

act::ref act::text(strc tx) {
  base::setText(toQt(tx)); RTHIS
}

str act::text() const {
  return fromQt(base::text());
}

act::ref act::tip(strc tx) {
  QString tip = toQt(tx), s = base::shortcut().toString();
  if (!s.isEmpty())
    tip += " [" + s + "]";
  base::setToolTip(tip); RTHIS
}

act::ref act::key(strc key) {
  base::setShortcut(QKeySequence(toQt(key)));
  tip(fromQt(base::toolTip())); RTHIS
}

act::ref act::icon(strc fileName) {
  base::setIcon(QIcon(toQt(fileName))); RTHIS
}

act::ref act::setCheckable(bool on) {
  base::setCheckable(on); RTHIS
}

bool act::isCheckable() const {
  return base::isCheckable();
}

act::ref act::check(bool on) {
  if (isCheckable())
    base::setChecked(on);
  else
    base::trigger();

  RTHIS
}

//------------------------------------------------------------------------------

actbtn::actbtn() {
  base::setToolButtonStyle(Qt::ToolButtonIconOnly);
}

actbtn::actbtn(act& a) {
  base::setToolButtonStyle(Qt::ToolButtonIconOnly);
  action(&a);
}

actbtn::ref actbtn::action(act *a) {
  base::setDefaultAction(a); RTHIS
}

//------------------------------------------------------------------------------

acts::acts(win& w_) : w(w_)  {
  auto mb = w.menuBar();
  mb->setNativeMenuBar(false);
  mb->setFixedHeight(0); // "hide"

  auto a = act::make("Quit", "Ctrl+Q");
  mut(*a).onTrigger([this]() {
    w.close();
  });

  add(QUIT, a);
}

acts::ref acts::add(strc hashKey, l::give_me<act> a) {
  as.add(hashKey, a.ptr());

//TODO  w.menuBar()->addAction(a.ptr());
  RTHIS
}

act& acts::get(strc hashKey) const may_err {
  return mut(*(as.at(hashKey)));
}

str const acts::QUIT("quit");

//------------------------------------------------------------------------------
}
// eof