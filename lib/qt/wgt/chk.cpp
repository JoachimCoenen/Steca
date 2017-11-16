// (lib/qt)

#include "chk.hpp"
#include "../defs.inc"
#include "../app/act.hpp"

namespace l_qt {
//------------------------------------------------------------------------------

chk_base::chk_base(strc t) {
  text(t);
}

chk_base::ref chk_base::text(strc t)
  SET_(base::setText(toQt(t)))

//------------------------------------------------------------------------------

chk::chk(strc tx) : base(tx) {
}

chk::chk(act& act, strc tx) : base(tx) {
  action(&act);
}

chk::ref chk::action(act* a) {
  base::setText(a ? toQt(a->text()).toLower() : QString::null);

  disconnect(con1);
  disconnect(con2);

  if (a) {
    base::setToolTip(a->toolTip());
    base::setChecked(a->isChecked());

    con1 = connect(this, &Self::toggled, [a](bool on) {
      a->check(on);
    });

    con2 = connect(a, &act::toggled, [this](bool on) {
      check(on);
    });
  }

  RTHIS
}

chk::ref chk::check(bool on)
  SET_(base::setChecked(on))

//------------------------------------------------------------------------------

triChk::triChk(strc s) : base (s) {
  setTristate(true);
  connect(this, &Self::clicked, [this]() {
    if (eState::part == get())
      set(eState::on); // clicking on 'off' -> 'on'
  });
}

TEST_("triState",
  CHECK_EQ(triChk::eState::off,  triChk::eState(Qt::Unchecked));
  CHECK_EQ(triChk::eState::part, triChk::eState(Qt::PartiallyChecked));
  CHECK_EQ(triChk::eState::on,   triChk::eState(Qt::Checked));
)

triChk::ref triChk::set(eState st)
  SET_(setCheckState(Qt::CheckState(st)))

triChk::eState triChk::get() const
  RET_(eState(checkState()))

//------------------------------------------------------------------------------
}
// eof DOCS
