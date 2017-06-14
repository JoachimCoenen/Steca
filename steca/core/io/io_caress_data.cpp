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

#include "io_caress_data.h"
#include <dev_lib/inc/defs_cpp.h>

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
    EXPECT_(false)
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
    EXPECT_(false)
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

  l::err(CAT("unsuported data type: ", t));
}


static mem getUnit(uint n, sz_t sz) {
  mem data(n * sz);
  if (n > 0)
    check_or_err_(0 == get_data_unit(data.data()), "bad data unit");
  return data;
}

static mem getPartition(uint n, sz_t sz, int32 d_type) {
  mem data(n * sz);

  int32 start = 1, ni = int32(n), section = 1;    // Is there ever section other than 1 ?
  while (ni > 0) {
    mem buf(MAXNUMBEROFCHANNELS * sz);
    int32 n = l::min(ni, MAXNUMBEROFCHANNELS);
    check_or_err_(0 == get_data_partition(buf.data(), &section, &start, &n, &d_type), "bad data partition");
    EXPECT_(n == l::min(ni, MAXNUMBEROFCHANNELS)) // Why on Earth is it passed by * ?
    memmove(data.data() + l::to_u(start-1)*sz, buf.data(), l::to_u(n));
    ni -= n; start += n;
  }

  return data;
}

bool openFile(strc f) {
  return OK == open_data_file(f.c_str(), nullptr);
}

void closeFile() {
  close_data_file();
}

mem getData(dtype dt, uint n) {
  auto sz = dtype_size(dt);
  if (n > MAXNUMBEROFCHANNELS)
    return getPartition(n, sz, from_dtype(dt));
  else
    return getUnit(n, sz);
}

str getString(uint n) {
  return getData(CHR, n).data();
}

str getAsString(dtype dt, uint n) {
  if (CHR == dt)
    return getString(n);

  check_or_err_(1 == n, "bad n<>1");
  auto m = getData(dt, n);

  switch (dt) {
  case NONE:
    return nullstr;
  case INT16:
    return std::to_string(*reinterpret_cast<int16 const*>(m.data()));
  case INT32:
    return std::to_string(*reinterpret_cast<int32 const*>(m.data()));
  case FLT32:
    return std::to_string(*reinterpret_cast<flt32 const*>(m.data()));
  default:
    EXPECT_(false)
    return nullstr;
  }
}

float getAsFloat(dtype dt, uint n) {
  switch (dt) {
  case NONE:
  case CHR:
    l::err("cannot get as float");
  default:
    break;
  }

  check_or_err_(1 == n, "bad n<>1");

  auto m = getData(dt, n);

  switch (dt) {
  case INT16:
    return float(*reinterpret_cast<int16 const*>(m.data()));
  case INT32:
    return float(*reinterpret_cast<int32 const*>(m.data()));
  case FLT32:
    return float(*reinterpret_cast<flt32 const*>(m.data()));
  default:
    EXPECT_(false)
    return 0;
  }
}

bool nextDataUnit(str& elem, str& node, dtype& dt, uint& n) {
  int32 e_number, e_type, d_number, d_type;
  modname_t m_elem, m_node;
  switch (next_data_unit(&e_number, &e_type, m_elem, m_node, &d_type, &d_number)) {
  case OK: {
    dt = to_dtype(d_type);
    check_or_err_(d_number >= 0, "bad d_number");
    n = l::to_uint(d_number);

    elem = l::trim(str(m_elem, MAXNAMELENGTH));
    node = l::trim(str(m_node, MAXNAMELENGTH));

    return true;
  }
  case 2: // END_OF_FILE_DETECTED (raw.cpp)
    return false;
  default:
    l::err("bad next_data_unit");
  }
}

l::vec<int32> getAdet(dtype dt, uint n) {
  check_or_err_(INT32==dt, "expecting int32 adet");

  auto m = getData(dt, n);
  l::vec<int32> v(n, 0);

  auto d = v.data();
  auto s = reinterpret_cast<int32 const*>(m.data());

  for_i_(n)
    *d++ = *s++;

  return v;
}

//------------------------------------------------------------------------------
}}
// eof
