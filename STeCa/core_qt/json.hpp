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

#ifndef CORE_JSON_H
#define CORE_JSON_H

#include <c2/c/c_c.h>
#include <c2/c/lib/num.h>
#include <c2/c/lib/ptr.h>

#include <c2/cpp/exc.hpp>
#include <c2/gui_qt/def.hpp>

#include <c2/typ/ij.h>
#include <c2/typ/xy.h>
#include <core/typ/range.hpp>
#include <core/typ/fun.hpp>

#include <QJsonArray>
#include <QJsonObject>

namespace core {
//------------------------------------------------------------------------------

struct JsonArr;

def_struct_reimpl (JsonObj, QJsonObject)
  using pint  = c::pint;
  using preal = c::preal;

  JsonObj();
  JsonObj(QJsonObject const&);

  // plain types
  JsonObj& saveObj(qstrc key, JsonObj const&);
  JsonObj  loadObj(qstrc key, bool defEmpty=false) const may_err;

  JsonObj& saveArr(qstrc key, JsonArr const&);
  JsonArr  loadArr(qstrc key, bool defEmpty=false) const may_err;

  JsonObj& saveInt(qstrc key, int);
  int      loadInt(qstrc key)               const may_err;
  int      loadInt(qstrc key, int def)      const may_err;

  JsonObj& saveUint(qstrc key, uint);
  uint     loadUint(qstrc key)              const may_err;
  uint     loadUint(qstrc key, uint def)    const may_err;

  JsonObj& savePint(qstrc key, pint);
  pint     loadPint(qstrc key)              const may_err;
  pint     loadPint(qstrc key, uint def)    const may_err;

  JsonObj& saveReal(qstrc key, real);
  real     loadReal(qstrc key)              const may_err;
  real     loadReal(qstrc key, real def)    const may_err;

  JsonObj& savePreal(qstrc key, preal);
  preal    loadPreal(qstrc key)             const may_err;
  preal    loadPreal(qstrc key, preal def)  const may_err;

  JsonObj& saveBool(qstrc key, bool);
  bool     loadBool(qstrc key)              const may_err;
  bool     loadBool(qstrc key, bool def)    const may_err;

  JsonObj& saveStr(qstrc key, qstrc);
  qstr     loadStr(qstrc key)               const may_err;
  qstr     loadStr(qstrc key, qstrc def)    const may_err;

  // more custom types
  JsonObj& saveIJ(qstrc key, c::ij::rc);
  c::ij    loadIJ(qstrc key)                const may_err;

  JsonObj& saveXY(qstrc key, c::xy::rc);
  c::xy    loadXY(qstrc key)                const may_err;

  JsonObj& saveRange(qstrc key, Range::rc);
  Range    loadRange(qstrc key)             const may_err;

  JsonObj& saveRanges(qstrc key, Ranges::rc);
  Ranges   loadRanges(qstrc key)            const may_err;

  JsonObj& operator+= (rc);
  JsonObj  operator+  (rc) const;

def_struct_end

//------------------------------------------------------------------------------

def_struct_reimpl (JsonArr, QJsonArray)
  JsonArr();
  JsonArr(QJsonArray const&);

  using base::append;
  void append(JsonObj::rc);

  using base::begin;
  using base::end;
  using base::constBegin;
  using base::constEnd;

  uint  count() const;
  JsonObj objAt(uint) const;

def_struct_end

//------------------------------------------------------------------------------
}
#endif
