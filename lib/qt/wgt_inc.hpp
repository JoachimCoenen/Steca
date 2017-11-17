// (lib/qt)

/**@file
 * Include all simple widgets; provide helper functions that make the widgets.
*/

#pragma once

// selected widgets
#include "wgt/btn.hpp"
#include "wgt/cbo.hpp"
#include "wgt/chk.hpp"
#include "wgt/edit.hpp"
#include "wgt/font.hpp"
#include "wgt/ico.hpp"
#include "wgt/lbl.hpp"
#include "wgt/rio.hpp"
#include "wgt/spin.hpp"

namespace l_qt { namespace make_widgets {
//------------------------------------------------------------------------------

l_qt::btn*      btn(l_qt::act&);
l_qt::pushbtn*  pushbtn(strc tx);

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

