// (qt_lib)

#pragma once
#include <dev_lib/defs.hpp>
#include <QCheckBox>

namespace l_qt {
//------------------------------------------------------------------------------

struct act;

dcl_sub_(chk, QCheckBox)
  chk(strc);
  chk(act&);

  set_(text,   (strc));
  set_(action, (act*));
  set_(check,  (bool = true));

protected:
  QMetaObject::Connection con1, con2;
dcl_end

//------------------------------------------------------------------------------
}
// eof
