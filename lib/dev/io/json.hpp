// (dev_lib)

/* Adapted from https://github.com/dropbox/json11. Original license:
 *
 * Copyright (c) 2013 Dropbox, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once
#include "../defs.hpp"
#include "../inc/str.hpp"
#include "../typ/vec.hpp"
#include "../typ/map.hpp"
#include <iostream>

namespace l {

struct ij;
struct xy;

}

namespace l_io {
//------------------------------------------------------------------------------

dcl_base_(Json)
  enum Typ {
    NUM, STR, VEC, OBJ,
  };

  dcl_(indent_t)
    atr_(uint, depth);
    atr_(bool, newLine);

    indent_t();
    mth_(indent_t, next, (bool newLine));
  dcl_end

  dcl_base_(Val)
    atr_(Typ, typ);
    Val(Typ);
    virtual voi_(saveTo, (std::ostream&, indent_t)) = 0;
  dcl_end

  template <Typ Ttyp, typename Tval>
  dcl_sub_(TypVal, Val)
    atr_(Tval, val);
    TypVal(Tval const& val_) : base(Ttyp), val(val_) {}
  dcl_end

  dcl_sub_(ValNum, TypVal<NUM COMMA flt32>)
    using base::base;
    voi_(saveTo, (std::ostream&, indent_t));
  dcl_end

  dcl_sub_(ValStr, TypVal<STR COMMA str>)
    using base::base;
    voi_(saveTo, (std::ostream&, indent_t));
  dcl_end

  using Vec = l::vec<Json>;

  dcl_sub_(ValVec, TypVal<VEC COMMA Vec>)
    using base::base;
    voi_(saveTo, (std::ostream&, indent_t));
  dcl_end

  using Obj = l::map<str, Json>;

  dcl_sub_(ValObj, TypVal<OBJ COMMA Obj>)
    using base::base;
    voi_(saveTo, (std::ostream&, indent_t));
  dcl_end

  // data
  atr_(l::shr<Val>, val);

  Json(Typ);
  Json(rc) = default;

  explicit Json(flt32);
  explicit Json(strc);
  explicit Json(Vec::rc);
  explicit Json(Obj::rc);

  explicit Json(strc, Json::rc);

  set_(add, (strc key, rc that)) may_err;
  set_(add, (rc that))           may_err;

  mth_(Self, operator+, (rc that)) may_err;

  mth_(sz_t, size, ())   may_err;
  mth_(rc,   at, (uint)) may_err;
  mth_(rc,   at, (strc)) may_err;

  mth_(int,     asInt,  ()) may_err;
  mth_(uint,    asUint, ()) may_err;
  mth_(flt32,   asFlt,  ()) may_err;
  mth_(strc,    asStr,  ()) may_err;
  mth_(Vec::rc, asVec,  ()) may_err;
  mth_(Obj::rc, asObj,  ()) may_err;

  voi_(saveTo, (std::ostream&));
  voi_(saveTo, (std::ostream&, indent_t));

  fry_(Self, loadFrom, (std::istream&)) may_err;

  fry_(Self, loadNum, (std::istream&)) may_err;
  fry_(Self, loadStr, (std::istream&)) may_err;
  fry_(Self, loadVec, (std::istream&)) may_err;
  fry_(Self, loadObj, (std::istream&)) may_err;

  explicit Json(l::ij const&);
  mth_(l::ij, asIJ, ()) may_err;

  explicit Json(l::xy const&);
  mth_(l::xy, asXY, ()) may_err;
dcl_end

//------------------------------------------------------------------------------
}
// eof
