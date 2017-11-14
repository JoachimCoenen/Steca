// (qt_lib)

#pragma once
#include "../defs.hpp"
#include <QPushButton>
#include <QToolButton>

namespace l_qt {
//------------------------------------------------------------------------------

struct act;

dcl_sub_(btn, QToolButton)
  btn();
  btn(act&);
  set_(action, (act*));
dcl_end

dcl_sub_(pushbtn, QPushButton)
  pushbtn();
  pushbtn(strc);
  set_(text, (strc));

  template <typename L>
  set_(onClick, (L l))
    SET_(connect(this, &QPushButton::clicked, l))
dcl_end

//------------------------------------------------------------------------------
}
// eof DOCS
