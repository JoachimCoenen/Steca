// (app_lib)

#include "act.h"
#include "../inc/defs_cpp.h"

namespace l_qt {
//------------------------------------------------------------------------------

act::act() : act(str::null) {}

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
  base::setText(tx);
  return *this;
}

str act::text() const {
  return base::text();
}

act::ref act::tip(strc tx) {
  str tip = tx, s = base::shortcut().toString();
  if (!s.isEmpty())
    tip += " [" + s + "]";
  base::setToolTip(tip);
  return *this;
}

act::ref act::key(strc key) {
  base::setShortcut(QKeySequence(key));
  tip(base::toolTip());
  return *this;
}

act::ref act::icon(strc fileName) {
  base::setIcon(QIcon(fileName));
  return *this;
}

act::ref act::checkable(bool on) {
  base::setCheckable(on);
  return *this;
}

bool act::isCheckable() const {
  return base::isCheckable();
}

act::ref act::check(bool on) {
  if (isCheckable())
    base::setChecked(on);
  else
    base::trigger();

  return *this;
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
  base::setDefaultAction(a);
  return *this;
}

//TODO//------------------------------------------------------------------------------

//acts::acts(win& w_) : w(w_)  {
//  auto mb = w.qt().menuBar();
//  mb->setNativeMenuBar(false);
//  mb->setFixedHeight(0); // "hide"

//  auto a = act::make("Quit", "Ctrl+Q");
//  mut(*a).onTrigger([this]() {
//    w.qt().close();
//  });

//  add(QUIT, a);
//}

//void acts::add(strc hashKey, c::give_me<act> a) {
//  auto ap = a.ptr();
//  as.add(hashKey, ap);

//  auto mb = w.qt().menuBar();
//  mb->addAction(ap);
//}

//act& acts::get(strc hashKey) {
//  return mut(*(as.at(hashKey).ptr()));
//}

//str const acts::QUIT("quit");

//------------------------------------------------------------------------------
}
// eof
