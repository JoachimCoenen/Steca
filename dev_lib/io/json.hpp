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
#include "../inc/ptr.hpp"
#include "../typ/vec.hpp"
#include "../typ/map.hpp"

#include <iostream>

namespace l {
//------------------------------------------------------------------------------

dcl_(Json)
  enum Typ {
    NUM, STR, VEC, OBJ,
  };

  dcl_base_(Val)
    atr_(Typ, typ);
    Val(Typ);
    virtual mth_(void, saveTo, (std::ostream&, uint indent)) = 0;
  dcl_end

  template <Typ Ttyp, typename Tval>
  dcl_sub_(TypVal, Val)
    atr_(Tval, val);
    TypVal(Tval const& val_) : base(Ttyp), val(val_) {}
  dcl_end

  dcl_sub_(ValNum, TypVal<NUM COMMA flt32>)
    using base::base;
    mth_(void, saveTo, (std::ostream&, uint indent));
  dcl_end

  dcl_sub_(ValStr, TypVal<STR COMMA str>)
    using base::base;
    mth_(void, saveTo, (std::ostream&, uint indent));
  dcl_end

  using Vec = vec<Json>;

  dcl_sub_(ValVec, TypVal<VEC COMMA Vec>)
    using base::base;
    mth_(void, saveTo, (std::ostream&, uint indent));
  dcl_end

  using Obj = map<str, Json>;

  dcl_sub_(ValObj, TypVal<OBJ COMMA Obj>)
    using base::base;
    mth_(void, saveTo, (std::ostream&, uint indent));
  dcl_end

  // data
  atr_(shared<Val>, val);

  explicit Json(flt32);
  explicit Json(strc);
  explicit Json(Vec::rc);
  explicit Json(Obj::rc);

  mth_(int,     asInt, ());
  mth_(flt32,   asFlt, ());
  mth_(strc,    asStr, ());
  mth_(Vec::rc, asVec, ());
  mth_(Obj::rc, asObj, ());

  mth_(void, saveTo,   (std::ostream&, uint indent = 0));
  fry_(Json, loadFrom, (std::istream&)) may_err;

  fry_(Json, loadNum, (std::istream&)) may_err;
  fry_(Json, loadStr, (std::istream&)) may_err;
  fry_(Json, loadVec, (std::istream&)) may_err;
  fry_(Json, loadObj, (std::istream&)) may_err;

dcl_end

//------------------------------------------------------------------------------
}
// eof
