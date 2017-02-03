/*******************************************************************************
 * STeCa2 - StressTextureCalculator ver. 2
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

#include "io.hpp"
#include <c/c/cpp>
#include <c/c/lib/num.h>
#include <c/c/lib/str.h>
#include <c/c/lib/mem.h>
#include "Caress/raw.i"
#include <string.h>

/* Mirko Boin says:

... the status of the instrument before a measurement/scan ... refers to all
nodes in the CARESS file whose elementName equals "READ".
... the data associated with a scan step ... should refer to all nodes whose
elementName is "SETVALUE", "MASTER1V" or "PROTOCOL".
*/

namespace core { namespace io {
//------------------------------------------------------------------------------

void loadCaress(data::Files& files, strc filePath) may_exc {
  using c::str;

  struct _close_data_file { ~_close_data_file() { close_data_file(); } } _auto_close;
  if (0 /*OK*/ != open_data_file(filePath,nullptr))
    err("Cannot open ", filePath);

  modname_t element, node;

  int32 e_number, // element number
        e_type,   // element type
        d_type,   // data type
        d_number; // data length (?)

  auto gds = [&]() -> str {
    if (d_number < 1)
      c::err("bad d_number: ", element);
    c::mem m(c::to_u(d_number) + 1);

    if (0 != get_data_unit(mut(m.p)))
      c::err("bad: ", "string data");

    return str(pcstr(m.p));
  };

  auto gdf = [&]() -> float {
    float f;
    if (0 != get_data_unit(&f))
      c::err("bad: ", "float data");
    return f;
  };

  str s_masterCounter, s_date, s_comment;

  float tthAxis = 0, omgAxis = 0, chiAxis = 0, phiAxis = 0;
  bool  isRobot = false, isTable = false;

  auto checkRobot = [&]() {
    if (isTable)
      c::err("bad: ", "already have table");
    isRobot = true;
  };

  auto checkTable = [&]() {
    if (isRobot)
      c::err("bad: ", "already have robot");
    isTable = true;
  };

  for (;;) {
    auto resNextUnit = next_data_unit(&e_number, &e_type, element, node, &d_type, &d_number);
    if (2 /*END_OF_FILE_DETECTED*/ == resNextUnit)
      break;
    if (0 /*OK*/ != resNextUnit)
      err("Error in ", filePath);

    WT(element << node << e_number << e_type << d_type << d_number)

    str el(str(8, element).trim());
//    WT(el)
    str en(str(8, node).trim());
//    WT(en)

    if (el.eq("MM1")) {
      s_masterCounter.set(node);
      continue;
    }

    if (el.eq("COM")) {
      s_comment.set(gds());
      continue;
    }

    if (el.eq("DATE")) {
      s_date.set(gds());
      continue;
    }

    if (el.eq("MASTERV1") && en.eq("ADET")) {

      continue;
    }

    if (el.eq("READ") || el.eq("SETVALUE") || el.eq("MASTERV1") || el.eq("PROTOCOL")) {
      if (en.empty())
        err("empty node for: ", el);

      float f = gdf();

      mut(*files.dict).add(en);

      if (en.eq("TTHS")) {
        checkTable(); tthAxis = f;
      } else
      if (en.eq("OMGS")) {
        checkTable(); omgAxis = f;
      } else
      if (en.eq("CHIS")) {
        checkTable(); chiAxis = f;
      } else
      if (en.eq("PHIS")) {
        checkTable(); phiAxis = f;
      } else
      if (en.eq("TTHR")) {
        checkRobot(); tthAxis = f;
      } else
      if (en.eq("OMGR")) {
        checkRobot(); omgAxis = f;
      } else
      if (en.eq("CHIR")) {
        checkRobot(); chiAxis = 180 - f; // !
      } else
      if (en.eq("PHIR")) {
        checkRobot(); phiAxis = f;
      } else
      if (en.eq("TIM1")) {
        // if s_date y < 2015 || 2015 Jan/Feb : f = f / 100
      };
      continue;
    }
  }

  //  data::shp_File file(new data::File(filePath));
  //  return file;
  //}
}

//------------------------------------------------------------------------------
}}
// eof
