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

#include "model_view.hpp"
#include <lib/qt/defs.inc>

namespace gui {
//------------------------------------------------------------------------------

ModelReflections::ModelReflections() : rs(nullptr) {}

ModelReflections::cl_n ModelReflections::cols() const {
  return cl_n(2);
}

ModelReflections::rw_n ModelReflections::rows() const {
  return rw_n(rs ? rs->size() : 0);
}

str ModelReflections::head(cl_n cl) const {
  switch (cl) {
  case clID:   return "#";
  case clTYPE: return "type";
  }

  return str::null;
}

l_qt::var ModelReflections::cell(rw_n rw, cl_n cl) const {
  switch (cl) {
  case clID:   return rw + 1;
  case clTYPE: return name(rw);
  }

  return l_qt::var();
}

str ModelReflections::name(rw_n rw) const {
  return rs ? rs->at(rw)().peakFun->sType() : str::null;
}

str_vec ModelReflections::names() const {
  str_vec ns;
  for_i_(rows())
    ns.add(name(rw_n(i)));
  return ns;
}

//------------------------------------------------------------------------------
}
// eof
