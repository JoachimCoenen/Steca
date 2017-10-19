// (qt_lib)

#pragma once

// gui widgets
#include "act.hpp"
#include "btn.hpp"
#include "cbo.hpp"
#include "chk.hpp"
#include "edit.hpp"
#include "ico.hpp"
#include "layout.hpp"
#include "lbl.hpp"
#include "lst.hpp"
#include "rio.hpp"
#include "spin.hpp"
#include "split.hpp"
#include "tabs.hpp"

namespace l_qt { namespace make_widgets {
//------------------------------------------------------------------------------

// returns could be l::own<>, but we want r/w
l_qt::actbtn*   btn(l_qt::act&);
l_qt::lbl*      lbl(strc);
l_qt::ico*      ico(strc file);

l_qt::spinInt*  spinInt(uint digitWidth);
l_qt::spinUint* spinUint(uint digitWidth);
l_qt::spinPint* spinPint(uint digitWidth);
l_qt::spinReal* spinReal(uint digitWidth, uint decimals);

l_qt::cbo*      cbo();
l_qt::cbo*      cbo(str_vec const&);
l_qt::rio*      rio(strc);

l_qt::chk*      chk(strc = str::null);
l_qt::chk*      chk(l_qt::act&, strc = str::null);

l_qt::edit*     edit();

//------------------------------------------------------------------------------
}}
// eof

