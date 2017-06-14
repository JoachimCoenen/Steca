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

#include "io.h"
#include "io_caress_data.h"
#include <dev_lib/inc/defs_cpp.h>
#include <functional>
#include <cmath>
/*
 * This is our best attempt at wrapping nicely the "raw" Caress data handling
 * routines. jb.
 */

namespace core { namespace io {
//------------------------------------------------------------------------------

str loadCaressComment(l::path::rc) {
  return ""; // TODO
}

//------------------------------------------------------------------------------

using data::Files;
using data::File;
using data::Set;
using data::Meta;

using data::flt_vec;

static File::sh loadOpenCaressFile(Files& files, strc name) may_err {
  File::sh file(new File(files, name));

  enum class eAxes  { NONE, ROBOT, TABLE }
    axes = eAxes::NONE;
  enum class eBlock { NONE, READ, SETVALUE, MASTER1V }
    block = eBlock::NONE;

  std::function<void()> checkRobot = [&]() {
    check_or_err_(eAxes::TABLE != axes, "bad: already have table data");
    axes = eAxes::ROBOT;
  };

  std::function<void()> checkTable = [&]() {
    check_or_err_(eAxes::ROBOT != axes, "bad: already have robot data");
    axes = eAxes::TABLE;
  };

  str elem, node; dtype dt; uint n;

  flt_vec readVals, vals;

  auto setVal = [&](flt_vec& vs, uint idx, flt32 val) {
    if (idx+1 > vs.size())
      vs.resize(idx+1, 0);
    vs.setAt(idx, val);
  };

  auto addValTo = [&](flt_vec& vs, strc ns, flt32 val) -> uint {
    auto idx = mut(*files.dict).add(ns);
    setVal(vs, idx, val);
    return idx;
  };

  auto addVal = [&](flt_vec& vs) -> uint {
    return addValTo(vs, node, getAsFloat(dt, n));
  };

  flt32 tth = l::flt_nan, omg = l::flt_nan, chi = l::flt_nan, phi = l::flt_nan,
        tim = l::flt_nan, mon = l::flt_nan;

  l::scoped<Image> image;

  auto doAxis = [&](pcstr ns, std::function<void()> check, flt32& val) -> bool {
    if (node != ns)
      return false;
    check();
    val = getAsFloat(dt, n);
    return true;
  };

  auto doAxes = [&]() -> bool {
    return
      doAxis("TTHS", checkTable, tth) ||
      doAxis("OMGS", checkTable, omg) ||
      doAxis("CHIS", checkTable, chi) ||
      doAxis("PHIS", checkTable, phi) ||
      doAxis("TTHR", checkRobot, tth) ||
      doAxis("OMGR", checkRobot, omg) ||
      doAxis("CHIR", checkRobot, chi) ||
      doAxis("PHIR", checkRobot, phi);
  };

  auto doVal = [&](pcstr ns, flt32& val) -> bool {
    if (node != ns)
      return false;
    val = getAsFloat(dt, n);
    return true;
  };

  auto doTimMon = [&]() -> bool {
    return
      doVal("TIM1", tim) ||
      doVal("MON",  mon);
    // TODO (Michael): until (including) Feb 2015, tim /= 100
  };

  auto beginDataset = [&]() {
    vals = readVals;
  };

  flt32 lastTim = l::flt_nan, lastMon = l::flt_nan;

  auto endDataset = [&]() {
    if (vals.isEmpty())
      return;

    // angles
    check_or_err_(!l::isnan(tth), "missing TTH");
    bool robot = eAxes::ROBOT == axes;

    if (robot)
      chi = 180 - chi; // TODO ask Michael

    check_or_err_(image.ptr(), "do not have an image");

    check_or_err_(l::isnan(lastTim) || lastTim <= tim, "decreasing tim");
    check_or_err_(l::isnan(lastMon) || lastMon <= mon, "decreasing mon");

    flt32 dTim = tim - lastTim, dMon = mon - lastMon;
    mut(*file).addSet(
      l::share(new Set(
        l::share(new Meta(files.dict, vals, tth, omg, chi, phi, tim, mon, dTim, dMon)),
        l::share(image.take().ptr()))));

    vals.clear();
    lastTim = tim; lastMon = mon;
  };

  int imageSide = -1;

  while (nextDataUnit(elem, node, dt, n)) {
    if (elem == "READ") {

      // instrument state - global metadata
      check_or_err_(block <= eBlock::READ, "unexpect READ block");
      block = eBlock::READ;
      check_or_err_(!node.empty(), "empty READ node");
      if (!doAxes() && !doTimMon())
        addVal(readVals);

    } else if (elem == "SETVALUE") {

      // begin scan
      if (block != eBlock::SETVALUE) {
        endDataset();
        block = eBlock::SETVALUE;
        beginDataset();
      }

      check_or_err_(!node.empty(), "empty SETVALUE node");
      addVal(vals);

    } else if (elem == "MASTER1V") {

      // scan data
      block = eBlock::MASTER1V;

      check_or_err_(!node.empty(), "empty MASTER1V node");
      if (node == "ADET") {
        auto adet = getAdet(dt, n);
        auto size = adet.size();
        auto side = l::to_uint(l::floor(sqrt(size)));
        check_or_err_((side*side == size) && (imageSide < 0 || imageSide == int(side)),
                      "bad image size");
        imageSide = side;

        count_arr2 cs(l::sz2(side, side));
        for_i_(size)
          cs.setAt(i, inten_t(adet.at(i)));

        image.reset(new Image(cs));
      } else {
        addVal(vals);
      }

    } else {

      endDataset();
      if (node.empty()) { // file-level info
        str s(getAsString(dt, n));
        if (elem == "COM")
          mut(file->comment) = s;
        else
          mut(file->strs).add(std::make_pair(elem, s));
      }
      // anything else is ignored
    }
  }

  endDataset();

  files.addFile(file);
  return file;
}

File::sh loadCaress(Files& files, l::path::rc path) may_err {
  check_or_err_(openFile(path), CAT("Cannot open ", path));

  struct __ { ~__() { closeFile(); } } autoClose;

  try {
    return loadOpenCaressFile(files, path.basename());
  } catch (l::exc& e) {
    mut(e.msg) = CAT(path, e.msg);
    throw;
  }
}

//------------------------------------------------------------------------------
}}
// eof
