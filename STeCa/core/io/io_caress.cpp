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
#include "io_caress_data.hpp"
#include <c2/inc/c_cpp>

/*
 * This is our best attempt at wrapping nicely the "raw" Caress data handling
 * routines. jb.
 */

namespace core { namespace io {
//------------------------------------------------------------------------------

static data::File::sh loadCaress(data::Files& files) may_err {
  data::File::sh file(new data::File(files));

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

  c::str elem, node; dtype dt; uint n;
  while (nextDataUnit(elem, node, dt, n)) {
    TR(elem << node << dt << n)

    if (elem.eqi("READ")) {             // global metadata
      check_or_err (!node.isEmpty(), "empty node");
      mut(*files.dict).add(node);
    } else if (elem.eqi("SETVALUE")) {  // begin scan
    } else if (elem.eqi("MASTER1V")) {  // data
      if (node.eqi("ADET")) {

      }
    } else {                           // anything else
      if (node.isEmpty()) // otherwise ignored
        mut(file->strs).add(std::make_pair(elem, getAsString(n, dt))); // file info
    }


//    if (s_elem.eq("READ") || s_elem.eq("SETVALUE") || s_elem.eq("MASTERV1") || s_elem.eq("PROTOCOL")) {
//      check_or_err (!s_node.isEmpty(), "empty node for: ", s_elem);

//      float f = 0;//get1<flt32>();


//      if (s_node.eq("TTHS")) {
//        checkTable(); tthAxis = f;
//      } else
//      if (s_node.eq("OMGS")) {
//        checkTable(); omgAxis = f;
//      } else
//      if (s_node.eq("CHIS")) {
//        checkTable(); chiAxis = f;
//      } else
//      if (s_node.eq("PHIS")) {
//        checkTable(); phiAxis = f;
//      } else
//      if (s_node.eq("TTHR")) {
//        checkRobot(); tthAxis = f;
//      } else
//      if (s_node.eq("OMGR")) {
//        checkRobot(); omgAxis = f;
//      } else
//      if (s_node.eq("CHIR")) {
//        checkRobot(); chiAxis = 180 - f; // !
//      } else
//      if (s_node.eq("PHIR")) {
//        checkRobot(); phiAxis = f;
//      } else
//      if (s_node.eq("TIM1")) {
//        // if s_date y < 2015 || 2015 Jan/Feb : f = f / 100
//      };
//      continue;
//    }
  }

  files.addFile(file);
  return file;
}

data::File::sh loadCaress(data::Files& files, c::strc filePath) may_err {
  check_or_err (openFile(filePath), "Cannot open ", filePath);
  try {
    auto res = loadCaress(files);
    closeFile();
    return res;
  } catch (c::exc& e) {
    mut(e.msg).set(c::str::cat(filePath, e.msg));
    throw; // ?
  } catch (...) {
    closeFile();
    throw;
  }
}

//------------------------------------------------------------------------------
}}
// eof
