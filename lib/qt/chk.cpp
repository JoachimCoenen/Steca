// (qt_lib)

#include "chk.hpp"
#include "inc/defs.inc"
#include "act.hpp"

namespace l_qt {
//------------------------------------------------------------------------------

chk_base::chk_base(strc t) {
  text(t);
}

chk_base::ref chk_base::text(strc t)
  SET_(base::setText(toQt(t)))

//------------------------------------------------------------------------------

chk::chk(act& act) : base(str::null) {
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
}

TEST_("triState",
  CHECK_EQ(triChk::off,  triChk::state_t(Qt::Unchecked));
  CHECK_EQ(triChk::part, triChk::state_t(Qt::PartiallyChecked));
  CHECK_EQ(triChk::on,   triChk::state_t(Qt::Checked));
)

triChk::ref triChk::set(state_t st)
  SET_(setCheckState(Qt::CheckState(st)))

triChk::state_t triChk::get() const
  RET_(state_t(checkState()))

//------------------------------------------------------------------------------
}
// eof
