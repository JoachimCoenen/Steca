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
#include <functional>

/*
 * This is our best attempt at wrapping nicely the "raw" Caress data handling
 * routines. jb.
 */

namespace core { namespace io {
//------------------------------------------------------------------------------

using data::Files;
using data::File;
using data::Set;
using data::Meta;

using data::flt_vec;

static File::sh loadCaress(Files& files) may_err {

  File::sh file(new File(files));

  int  tthIdx = -1, omgIdx = -1, chiIdx = -1, phiIdx = -1;

  enum class eAxes  { NONE, ROBOT, TABLE }
    axes = eAxes::NONE;
  enum class eBlock { NONE, READ, SETVALUE, MASTER1V }
    block = eBlock::NONE;

  std::function<void()> checkRobot = [&]() {
    check_or_err (eAxes::TABLE != axes, "bad: ", "already have table data");
    axes = eAxes::ROBOT;
  };

  std::function<void()> checkTable = [&]() {
    check_or_err (eAxes::ROBOT != axes, "bad: ", "already have robot data");
    axes = eAxes::TABLE;
  };

  c::str elem, node; dtype dt; uint n;

  flt_vec readVals, vals;

  auto setVal = [&](flt_vec& vs, uint idx, flt32 val) {
    if (idx+1 > vs.size())
      vs.resize(idx+1, 0);
    vs.setAt(idx, val);
  };

  auto addValTo = [&](flt_vec& vs, c::strc ns, flt32 val) -> uint {
    auto idx = mut(*files.dict).add(ns);
    setVal(vs, idx, val);
    return idx;
  };

  auto addVal = [&](flt_vec& vs) -> uint {
    return addValTo(vs, node, getAsFloat(dt, n));
  };

  auto doAxis = [&](flt_vec& vs, pcstr ns, std::function<void()> check, int& idx) -> bool {
    if (!node.eqi(ns))
      return false;
    check();
    auto i = addVal(vs);
    if (idx < 0)
      idx = i;
    else
      EXPECT (i == idx)
    return true;
  };

  auto doAxes = [&](flt_vec& vs) -> bool {
    return
      doAxis(vs, "TTHS", checkTable, tthIdx) ||
      doAxis(vs, "OMGS", checkTable, omgIdx) ||
      doAxis(vs, "CHIS", checkTable, chiIdx) ||
      doAxis(vs, "PHIS", checkTable, phiIdx) ||
      doAxis(vs, "TTHR", checkRobot, tthIdx) ||
      doAxis(vs, "OMGR", checkRobot, omgIdx) ||
      doAxis(vs, "CHIR", checkRobot, chiIdx) ||
      doAxis(vs, "PHIR", checkRobot, phiIdx);
  };

  auto beginDataset = [&]() {
    vals = readVals;
  };

  auto endDataset = [&]() {
    if (vals.isEmpty())
      return;

    // angles
    check_or_err (tthIdx >= 0, "missing TTH");
    bool robot = eAxes::ROBOT == axes;

    flt32 tth = vals.at(tthIdx),
          omg = omgIdx >= 0 ? vals.at(omgIdx) : 0,
          chi = chiIdx >= 0 ? vals.at(chiIdx) : 0,
          phi = phiIdx >= 0 ? vals.at(phiIdx) : 0;

    if (robot)
      chi = 180 - chi; // TODO ask Michael

    mut(*file).addSet(
      c::share(new Set(
        c::share(new Meta(files.dict, vals, tth, omg, chi, phi)),
        c::share(new Image))));

    vals.clear();
  };

  while (nextDataUnit(elem, node, dt, n)) {
    if (elem.eqi("READ")) {

      // instrument state - global metadata
      check_or_err (block <= eBlock::READ, "unexpect READ block");
      block = eBlock::READ;
      check_or_err (!node.isEmpty(), "empty READ node");
      if (!doAxes(readVals))
        addVal(readVals);

    } else if (elem.eqi("SETVALUE")) {

      // begin scan
      if (block != eBlock::SETVALUE) {
        endDataset();
        block = eBlock::SETVALUE;
        beginDataset();
      }

      check_or_err (!node.isEmpty(), "empty SETVALUE node");
      addVal(vals);

    } else if (elem.eqi("MASTER1V")) {

      // scan data
      block = eBlock::MASTER1V;

      check_or_err (!node.isEmpty(), "empty MASTER1V node");
      if (node.eqi("ADET"))
          ; //        TR("adet")
      else
        addVal(vals);

    } else {

      endDataset();
      if (node.isEmpty()) // file-level info
        mut(file->strs).add(std::make_pair(elem, getAsString(dt, n)));

      // anything else is ignored

    }

//      if (s_node.eq("TIM1")) {
//        // if s_date y < 2015 || 2015 Jan/Feb : f = f / 100
//      };
  }

  endDataset();

  files.addFile(file);
  return file;
}

File::sh loadCaress(Files& files, c::strc filePath) may_err {
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
