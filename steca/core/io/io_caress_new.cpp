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
#include <dev_lib/defs.inc>
#include <dev_lib/io/fin.hpp>
#include <dev_lib/io/endian.hpp>

namespace core { namespace io {
//------------------------------------------------------------------------------

using data::Files;
using data::File;
using data::Set;
using data::Meta;

using l_io::path;

enum eDataType {
  NIL      = 0,
  INT16_LE = 1,
  INT16_BE = 9,
  INT32_LE = 2,
  INT32_BE = 10,
  INT64_LE = 13,
  INT64_BE = 14,
  //INTEGERTYPE = 1, /* short int (16 bit) */
  LONGINTEGER = 2,
  //REALTYPE = 5,    /* float (32 bit) */
  //DOUBLETYPE = 11, /* double (64 bit) */
  //INT64TYPE = 13,  /* long long (64 bit) */
  CHARTYPE = 16,
  //STRINGTYPE = 17, /* string ('\0' terminated) */
  //ADDRTYPE = 18,   /* address info */
  //MODIDTYPE = 19,  /* short int (module ID) */
  //ONOFFTYPE = 21,       /* OT: strings "ON" or "OFF" (e.g. command CHECK) */
  //PMATRIXTYPE = 22,     /* PT: strings "XX", "XY",..., "ZZ" (cmd PMATRIX) */
  //DAUTYPE = 23,         /* CT: strings "SI" or "MU" */
  //ENUMERATIONTYPE = 24, /* EN: indicating enumeration in xx_ranges.dat */
  //DAUDATATYPE = 25      /* IT: strings "I2" or "I4" in CONFIG command */
  PAREN    = 0x80,
  THESIS   = 0x81,
};

File::sh loadCaressNew(Files& files, path::rc path) may_err {

  File::sh file(new File(files, path.basename()));

  l_io::fbin fin(path);

  auto readInt = [&](uint8 type) -> int64 {
    switch (type) {
    case INT16_LE:
      return (int16)l_io::endian::from_le(fin.get16());
    case INT16_BE:
      return (int16)l_io::endian::from_be(fin.get16());
    case INT32_LE:
      return (int32)l_io::endian::from_le(fin.get32());
    case INT32_BE:
      return (int32)l_io::endian::from_be(fin.get32());
    case INT64_LE:
      return (int64)l_io::endian::from_le(fin.get64());
    case INT64_BE:
      return (int64)l_io::endian::from_be(fin.get64());
    default:
      fin.err("bad int");
    }
  };

  auto count = [&]() -> uint {
    auto c = fin.get8();
    return !(c & 0x80) ? c : l::to_uint(readInt(c & 0x7F));
  };

  auto readTypeItem = [&](uint8 type) {
    switch (type) {
    case NIL:
      break;
    case CHARTYPE: {
      auto cnt = count();
      TR_("str" << str(fin.read(cnt).data(), cnt))
      break;
    }
    case INT16_LE:
    case INT16_BE:
    case INT32_LE:
    case INT32_BE:
    case INT64_LE:
    case INT64_BE: {
      TR_("int" << readInt(type))
      break;
    }
    default:
      fin.err(CAT("unexpected item - ", type));
    }
  };

  auto readItem = [&]() {
    auto type = fin.get8();
    if (PAREN == type) {
      type   = fin.get8();
      auto thesis = fin.get8();
      if (THESIS != thesis)
        fin.err("nested types are not supported");
    }
    return readTypeItem(type);
  };

  for (;;)
    readItem();


//  auto charType = [&](uint count) -> str {
//    std::vector<char> s(count + 1, '\0');
//    is.read(s.data(), count);
//    return s.data();
//  };

//  for (;;) {
//    char type; is.get(type);

//    switch (type) {
//    case CHARTYPE:
//      WT_(charType(count()))
//      break;
//    case PAREN:
//      break;
//    default:
//      WT_(99)
//    }
//  }

}

TEST_("loadCaress",
  Files files;
  auto file = loadCaressNew(files, l_io::path("testdata.caress"));
)

//------------------------------------------------------------------------------
}}
// eof
