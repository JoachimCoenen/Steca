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

#include "io.hpp"
#include <lib/dev/inc/defs.inc>
#include "io_caress_data.hpp"
#include <functional>
/*
 * This is our best attempt at wrapping nicely the "raw" Caress data handling
 * routines. jb.
 */

namespace core { namespace io {
//------------------------------------------------------------------------------

str loadCaressComment(l_io::path::rc path) {
  str comment;

  try {
    check_or_err_(openFile(path), str::null);
    struct __ { ~__() { closeFile(); } } autoClose;

    str elem, node; dtype dt; uint n;
    while (nextDataUnit(elem, node, dt, n)) {
      if (elem == "COM") {
        comment = getAsString(dt, n);
        break;
      }
    }
  } catch (l::exc&) {}

  return comment;
}

//------------------------------------------------------------------------------

using data::Files;
using data::File;
using data::Set;
using data::FileSrc;
using data::Meta;
using data::MetaVals;

static l::own<File> loadOpenCaressFile(l_io::path::rc path) may_err {
  auto&& file = l::scope(new File(path));

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

  MetaVals readVals, vals;

  auto setVal = [&](MetaVals& vs, uint idx, flt32 val) {
    vs.setAt(idx, val);
  };

  auto addValTo = [&](MetaVals& vs, strc ns, flt32 val) -> uint {
    auto idx = mut(*file->dict).idxEnter(ns);
    setVal(vs, idx, val);
    return idx;
  };

  // prepare
  addValTo(readVals, "TTH", 0);
  addValTo(readVals, "OMG", 0);
  addValTo(readVals, "CHI", 0);
  addValTo(readVals, "PHI", 0);
  addValTo(readVals, "TIM1", 0);
  addValTo(readVals, "MON", 0);

  auto addVal = [&](MetaVals& vs) -> uint {
    return addValTo(vs, node, getAsFloat(dt, n));
  };

  // TODO make nanable flt32/64
  flt32 tth = l::flt32_nan, omg = l::flt32_nan, chi = l::flt32_nan, phi = l::flt32_nan,
        tim = l::flt32_nan, mon = l::flt32_nan;

  l::scoped<Image> image;

  auto doAxis = [&](pcstr ns, std::function<void()> check, flt32& val) -> bool {
    if (node != ns)
      return false;
    check();
    val = getAsFloat(dt, n);
    addValTo(readVals, str(ns).substr(0,3), val);
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
    addValTo(readVals, ns, val);
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

  flt32 lastTim = l::flt32_nan, lastMon = l::flt32_nan;

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
      l::sh(new Set(
        file->src,
        l::sh(new Meta(file->dict, vals, tth, omg, chi, phi, tim, mon, dTim, dMon)),
        l::sh(image.take()))));

    vals.clear();
    lastTim = tim; lastMon = mon;
  };

  int imageSide = -1;

  while (nextDataUnit(elem, node, dt, n)) {
    if (elem == "READ") {
      // instrument state - global metadata
      check_or_err_(block <= eBlock::READ, "unexpect READ block");
      block = eBlock::READ;
      check_or_err_(!node.isEmpty(), "empty READ node");

      doAxes() || doTimMon();
      addVal(readVals);

    } else if (elem == "SETVALUE") {

      // begin scan
      if (block != eBlock::SETVALUE) {
        endDataset();
        block = eBlock::SETVALUE;
        beginDataset();
      }

      check_or_err_(!node.isEmpty(), "empty SETVALUE node");
      addVal(vals);

    } else if (elem == "MASTER1V") {
      // scan data
      block = eBlock::MASTER1V;

      check_or_err_(!node.isEmpty(), "empty MASTER1V node");
      if (node == "ADET") {
        auto adet = getAdet(dt, n);
        auto size = adet.size();
        auto side = l::to_uint(l::floor(sqrt(size)));
        check_or_err_((side*side == size) && (imageSide < 0 || imageSide == int(side)),
                      "bad image size");
        imageSide = l::to_i(side);

        count_arr2 cs(l::sz2(side, side));
        for_i_(size)
          cs.setAt(i, inten_t(adet.at(i)));

        image.reset(new Image(cs));
      } else {
        addVal(vals);
      }
    } else {
      endDataset();
      if (node.isEmpty()) { // file-level info
        str s(getAsString(dt, n));
        if (elem == "COM")
          mut(file->src->comment) = s;
//        else // ignore other strings
//          mut(file->strs).add(std::make_pair(elem, s));
      }
      // anything else is ignored
    }
  }

  endDataset();

  return file.takeOwn();
}

l::own<File> loadCaress(l_io::path::rc path) may_err {
  check_or_err_(openFile(path), CAT("Cannot open ", path));
  struct __ { ~__() { closeFile(); } } autoClose;

  try {
    return loadOpenCaressFile(path);
  } catch (l::exc& e) {
    mut(e.msg) = CAT(path, e.msg);
    throw;
  }
}

TEST_("loadCaress",
  loadCaress(l_io::path("testdata.caress"));
)

//------------------------------------------------------------------------------
}}
// eof DOCS
