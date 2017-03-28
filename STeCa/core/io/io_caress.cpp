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
#include <c2/c/mem.h>
#include <c2/c/num.h>
#include <c2/c/str.h>
#include <c2/h/c_cpp>
#include <string.h>

#undef CHECK
#include "Caress/raw.h"

/* Mirko Boin says:
 * ... the status of the instrument before a measurement/scan ... refers to all
 * nodes in the CARESS file whose elementName equals "READ".
 * ... the data associated with a scan step ... should refer to all nodes whose
 * elementName is "SETVALUE", "MASTER1V" or "PROTOCOL".
*/

/*
 * This is our best attempt at wrapping nicely the "raw" Caress data handling
 * routines. jb.
 */

namespace core { namespace io {
//------------------------------------------------------------------------------

namespace get_data {
  c::mem unit(int32 n, sz_t sz) {
    check_or_err (n >= 0, "bad d_number");
    c::mem data(c::to_u(n) * sz);
    if (n > 0)
      check_or_err (0 == get_data_unit(mut(data.p)), "bad data unit");
    return data;
  };

  c::mem partition(int32 section, int32 n, sz_t sz, int32 d_type) {
    check_or_err (n > 0, "bad d_number");
    c::mem data(c::to_u(n) * sz);

    int32 start = 1;
    while (n > 0) {
      c::mem buf(MAXNUMBEROFCHANNELS * sz);
      int32 n_ = c::min(n, MAXNUMBEROFCHANNELS);
      check_or_err (0 == get_data_partition(mut(buf.p), &section, &start, &n_, &d_type), "bad data partition");
      check_or_err (n_ > 0, "bad n_");
      c::unsafe::memmov(pstr(data.p)+c::to_u(start-1)*sz, buf.p, c::to_u(n_));
      n -= n_; start += n_;
    }

    return data;
  };
}

c::mem getData(int32 n, sz_t sz, int32 d_type) {
  if (n > MAXNUMBEROFCHANNELS)
    return get_data::partition(1, n, sz, d_type);
  else
    return get_data::unit(n, sz);
}

static data::File::sh loadCaress() may_err {
  using c::str;

  modname_t element, node;

  int32 e_number, // element number
        e_type,   // element type
        d_type,   // data type
        d_number; // data length (?)

  str s_masterCounter, s_date, s_comment;

  float tthAxis = 0, omgAxis = 0, chiAxis = 0, phiAxis = 0;
  bool  isRobot = false, isTable = false;

  auto checkRobot = [&]() {
    check_or_err (!isTable, "bad: ", "already have table");
    isRobot = true;
  };

  auto checkTable = [&]() {
    check_or_err (!isRobot, "bad: ", "already have robot");
    isTable = true;
  };

  for (;;) {
    switch (next_data_unit(&e_number, &e_type, element, node, &d_type, &d_number)) {
    case 2: // END_OF_FILE_DETECTED
      goto exit;
    case 0:
      break;
    default:
      c::err("bad next_data_unit");
    }

    check_or_err (0 <= d_number, "bad d_number")
//    TR(element << node << e_number << e_type << d_type << d_number)

    switch (d_type) {
    case 1: {
      auto data = getData(d_number, sizeof(int16), d_type);
//      for_i (d_number < 12 ? d_number : 0)
//        TR('i' << reinterpret_cast<int16 const*>(data.p)[i])
      break;
    }

    case 2: {
      auto data = getData(d_number, sizeof(int32), d_type);
      for_i (d_number < 12 ? d_number : 0)
        TR('l' << reinterpret_cast<int32 const*>(data.p)[i])
      break;
    }

    case 5: {
      auto data = getData(d_number, sizeof(flt32), d_type);
      for_i (d_number < 12 ? d_number : 0)
        TR('f' << reinterpret_cast<flt32 const*>(data.p)[i])
      break;
    }

    case 16: {
      c::str s(getData(d_number, sizeof(char), d_type));
      TR('s' << s.p)
      break;
    }

    case 0: // nothing, just a tag
      break;

    default:
      c::err("not handled d_type");
    }

    str el(str(8, element).trim());
    str en(str(8, node).trim());

    if (el.eq("DAT")) {
//      TR(gds())
      continue;
    }

    if (el.eq("EXPTYPE")) {
//      TR(gds())
      continue;
    }

    if (el.eq("MM1")) {
      s_masterCounter.set(node);
      continue;
    }

    if (el.eq("COM")) {
//      s_comment.set(gds());
      continue;
    }

    if (el.eq("DATE")) {
//      s_date.set(gds());
      continue;
    }

    if (el.eq("MASTERV1") && en.eq("ADET")) {

      continue;
    }

    if (el.eq("READ") || el.eq("SETVALUE") || el.eq("MASTERV1") || el.eq("PROTOCOL")) {
      check_or_err (!en.isEmpty(), "empty node for: ", el);

      float f = 0;//get1<flt32>();

//      mut(*files.dict).add(en);

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

exit:
  data::File::sh file(new data::File);
  return file;
}

data::File::sh loadCaress(c::strc filePath) may_err {
  check_or_err (0 /*OK*/ == open_data_file(filePath, nullptr),
                "Cannot open ", filePath);
  try {
    auto res = loadCaress();
    close_data_file();
    return res;
  } catch (c::exc& e) {
    mut(e.msg).set(c::str::cat(filePath, e.msg));
    throw; // ?
  } catch (...) {
    close_data_file();
    throw;
  }
}

//------------------------------------------------------------------------------
}}
// eof
