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
#include <dev_lib/inc/defs.inc>
#include <fstream>

namespace core { namespace io {
//------------------------------------------------------------------------------

using data::Files;
using data::File;
using data::Set;
using data::Meta;

enum {
  CHARTYPE = 16,
  PAREN    = 0x80,
  THESIS   = 0x81,
};

File::sh loadCaressNew(Files& files, l::path::rc path) may_err {
  File::sh file(new File(files, path.basename()));

  std::ifstream is(path, std::ios::binary);

  auto check = [&is](strc msg) {
    WT_(is.fail())
    check_or_err_(!is.fail(), msg);
  };

  check("cannot open");

  auto count = [&]() -> uint {
    char byte; is.get(byte);

    if (!(byte & 0x80)) // immediate value
      return byte;

    switch (byte & 0x7F) {
    default:
      NEVER; // later
    }
  };

  auto charType = [&](uint count) -> str {
    std::vector<char> s(count + 1, '\0');
    is.read(s.data(), count);
    return s.data();
  };

  for (;;) {
    char type; is.get(type);

    switch (type) {
    case CHARTYPE:
      WT_(charType(count()))
      break;
    case PAREN:
      break;
    default:
      WT_(99)
    }
  }

}

//TEST_("loadCaress",
//  Files files;
//  auto file = loadCaressNew(files, l::path("testdata.caress"));
//)

//------------------------------------------------------------------------------
}}
// eof
