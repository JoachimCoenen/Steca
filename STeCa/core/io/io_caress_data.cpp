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

#include "io_caress_data.hpp"
#include <c2/c/num.h>
#include <c2/cpp/exc.hpp>
#include <c2/inc/c_cpp>
#undef CHECK
#include "Caress/raw.h"

namespace core { namespace io {
//------------------------------------------------------------------------------

static int32 from_dtype(dtype t) {
  switch (t) {
  case NONE:
    return 0;
  case INT16:
    return INTEGERTYPE;
  case INT32:
    return LONGINTEGER;
  case FLT32:
    return REALTYPE;
  case CHR:
    return CHARTYPE;
  default:
    EXPECT (false)
    return 0;
  }
}

static uint dtype_size(dtype t) {
  switch (t) {
  case INT16:
    return 2;
  case INT32:
    return 4;
  case FLT32:
    return 4;
  case CHR:
    return 1;
  default:
    EXPECT (false)
    return 0;
  }
}

static dtype to_dtype(int32 t) {
  switch (t) {
  case INTEGERTYPE:
    return INT16;
  case LONGINTEGER:
    return INT32;
  case REALTYPE:
    return FLT32;
  case CHARTYPE:
    return CHR;
  case 0:
    return NONE;
  }

  c::err("unsuported data type: ", t);
}


static c::mem getUnit(uint n, sz_t sz) {
  c::mem data(n * sz);
  if (n > 0)
    check_or_err (0 == get_data_unit(mut(data.p)), "bad data unit");
  return data;
}

static c::mem getPartition(uint n, sz_t sz, int32 d_type) {
  c::mem data(n * sz);

  int32 start = 1, ni = int32(n), section = 1;    // Is there ever section other than 1 ?
  while (ni > 0) {
    c::mem buf(MAXNUMBEROFCHANNELS * sz);
    int32 n = c::min(ni, MAXNUMBEROFCHANNELS);
    check_or_err (0 == get_data_partition(mut(buf.p), &section, &start, &n, &d_type), "bad data partition");
    EXPECT (n == c::min(ni, MAXNUMBEROFCHANNELS)) // Why on Earth is it passed by * ?
    c::unsafe::memmov(pstr(data.p)+c::to_u(start-1)*sz, buf.p, c::to_u(n));
    ni -= n; start += n;
  }

  return data;
}

bool openFile(c::strc f) {
  return OK == open_data_file(f.p, nullptr);
}

void closeFile() {
  close_data_file();
}

c::mem getData(dtype dt, uint n) {
  auto sz = dtype_size(dt);
  if (n > MAXNUMBEROFCHANNELS)
    return getPartition(n, sz, from_dtype(dt));
  else
    return getUnit(n, sz);
}

c::str getString(uint n) {
  return c::str(getData(CHR, n));
}

c::str getAsString(dtype dt, uint n) {
  if (CHR == dt)
    return getString(n);

  check_or_err (1 == n, "bad n<>1");
  auto m = getData(dt, n);

  switch (dt) {
  case NONE:
    return c::str::nul;
  case INT16:
    return c::str::frm(*static_cast<int16 const*>(m.p));
  case INT32:
    return c::str::frm(*static_cast<int32 const*>(m.p));
  case FLT32:
    return c::str::frm(*static_cast<flt32 const*>(m.p));
  default:
    EXPECT (false)
    return c::str::nul;
  }
}

float getAsFloat(dtype dt, uint n) {
  switch (dt) {
  case NONE:
  case CHR:
    c::err("cannot get as float");
  default:
    break;
  }

  check_or_err (1 == n, "bad n<>1");

  auto m = getData(dt, n);

  switch (dt) {
  case INT16:
    return float(*static_cast<int16 const*>(m.p));
  case INT32:
    return float(*static_cast<int32 const*>(m.p));
  case FLT32:
    return float(*static_cast<flt32 const*>(m.p));
  default:
    EXPECT (false)
    return 0;
  }
}

bool nextDataUnit(c::str& elem, c::str& node, dtype& dt, uint& n) {
  int32 e_number, e_type, d_number, d_type;
  modname_t m_elem, m_node;
  switch (next_data_unit(&e_number, &e_type, m_elem, m_node, &d_type, &d_number)) {
  case OK: {
    dt = to_dtype(d_type);
    check_or_err (d_number >= 0, "bad d_number");
    n = c::to_uint(d_number);

    elem.set(c::str(MAXNAMELENGTH, m_elem).trim());
    node.set(c::str(MAXNAMELENGTH, m_node).trim());

    return true;
  }
  case 2: // END_OF_FILE_DETECTED (raw.cpp)
    return false;
  default:
    c::err("bad next_data_unit");
  }
}

c::vec<int32> getAdet(dtype dt, uint n) {
  check_or_err (INT32==dt, "expecting int32 adet");

  auto m = getData(dt, n);
  c::vec<int32> v(n, 0);

  auto d = v.data();
  auto s = static_cast<int32 const*>(m.p);

  for_i (n)
    *d++ = *s++;

  return v;
}

//------------------------------------------------------------------------------
}}
// eof
