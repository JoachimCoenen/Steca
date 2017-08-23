// (qt_lib)

#pragma once
#include <lib/dev/defs.hpp>
#include <QCheckBox>

namespace l_qt {
//------------------------------------------------------------------------------

struct act;

dcl_sub_(chk_base, QCheckBox)
  chk_base(strc);

  set_(text, (strc));
dcl_end

dcl_sub_(chk, chk_base)
  using base::base;
  chk(act&);

  set_(action, (act*));
  set_(check,  (bool = true));

protected:
  QMetaObject::Connection con1, con2;
dcl_end

dcl_sub_(triChk, chk_base)
  triChk(strc);

  enum state_t {off, part, on};
  set_(set, (state_t));
  mth_(state_t, get, ());

dcl_end

//------------------------------------------------------------------------------
}
// eof
