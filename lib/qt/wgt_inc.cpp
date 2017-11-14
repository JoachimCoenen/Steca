/*******************************************************************************
 * Steca2 - StressTextureCalculator ver. 2
 *
 * Copyright (C) 2016-7 Forschungszentrum Jülich GmbH
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the COPYING and AUTHORS files for more details.
 ******************************************************************************/

#include "wgt_inc.hpp"

namespace l_qt { namespace make_widgets {
//------------------------------------------------------------------------------

l_qt::btn* btn(l_qt::act& act) {
  return new l_qt::btn(act);
}

l_qt::pushbtn* pushbtn(strc tx) {
  return new l_qt::pushbtn(tx);
}

l_qt::lbl* lbl(strc tx) {
  return new l_qt::lbl(tx);
}

l_qt::ico* ico(strc file) {
  return new l_qt::ico(file);
}

l_qt::spinInt* spinInt(uint w) {
  return new l_qt::spinInt(w);
}

l_qt::spinUint* spinUint(uint w) {
  return new l_qt::spinUint(w);
}

l_qt::spinPint* spinPint(uint w) {
  return new l_qt::spinPint(w);
}

l_qt::spinReal* spinReal(uint w, uint d) {
  return new l_qt::spinReal(w, d);
}

l_qt::cbo* cbo() {
  return new l_qt::cbo();
}

l_qt::cbo* cbo(str_vec const& ss) {
  return new l_qt::cbo(ss);
}

l_qt::rio* rio(strc tx) {
  return new l_qt::rio(tx);
}

l_qt::chk* chk(strc tx) {
  return new l_qt::chk(tx);
}

l_qt::chk* chk(l_qt::act& a, strc tx) {
  return new l_qt::chk(a, tx);
}

l_qt::edit* edit() {
  return new l_qt::edit();
}

//------------------------------------------------------------------------------
}}
// eof DOCS
