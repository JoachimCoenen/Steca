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

#include <c/c/c_c.h>
#include <c/c/lib/num.h>
#include <c/c/lib/ptr.h>

#include <c/cpp/exc.hpp>
#include <c/qt/def.hpp>

#include <core/typ/ij.h>
#include <core/typ/range.hpp>
#include <core/typ/xy.h>
#include <core/typ/fun.hpp>

#include <QJsonArray>
#include <QJsonObject>

namespace core {
//------------------------------------------------------------------------------

struct JsonArr;

sub_struct_reimpl (JsonObj, QJsonObject)
  using pint  = c::pint;
  using preal = c::preal;

  JsonObj();
  JsonObj(QJsonObject const&);

  // plain types
  JsonObj& saveObj(qstrc key, JsonObj const&);
  JsonObj  loadObj(qstrc key, bool defEmpty=false) const may_exc;

  JsonObj& saveArr(qstrc key, JsonArr const&);
  JsonArr  loadArr(qstrc key, bool defEmpty=false) const may_exc;

  JsonObj& saveInt(qstrc key, int);
  int      loadInt(qstrc key)               const may_exc;
  int      loadInt(qstrc key, int def)      const may_exc;

  JsonObj& saveUint(qstrc key, uint);
  uint     loadUint(qstrc key)              const may_exc;
  uint     loadUint(qstrc key, uint def)    const may_exc;

  JsonObj& savePint(qstrc key, pint);
  pint     loadPint(qstrc key)              const may_exc;
  pint     loadPint(qstrc key, uint def)    const may_exc;

  JsonObj& saveReal(qstrc key, real);
  real     loadReal(qstrc key)              const may_exc;
  real     loadReal(qstrc key, real def)    const may_exc;

  JsonObj& savePreal(qstrc key, preal);
  preal    loadPreal(qstrc key)             const may_exc;
  preal    loadPreal(qstrc key, preal def)  const may_exc;

  JsonObj& saveBool(qstrc key, bool);
  bool     loadBool(qstrc key)              const may_exc;
  bool     loadBool(qstrc key, bool def)    const may_exc;

  JsonObj& saveStr(qstrc key, qstrc);
  qstr     loadStr(qstrc key)               const may_exc;
  qstr     loadStr(qstrc key, qstrc def)    const may_exc;

  // more custom types
  JsonObj& saveIJ(qstrc key, IJ::rc);
  IJ       loadIJ(qstrc key)                const may_exc;

  JsonObj& saveXY(qstrc key, XY::rc);
  XY       loadXY(qstrc key)                const may_exc;

  JsonObj& saveRange(qstrc key, Range::rc);
  Range    loadRange(qstrc key)             const may_exc;

  JsonObj& saveRanges(qstrc key, Ranges::rc);
  Ranges   loadRanges(qstrc key)            const may_exc;

  JsonObj& operator+= (rc);
  JsonObj  operator+  (rc) const;

sub_struct_end

//------------------------------------------------------------------------------

sub_struct_reimpl (JsonArr, QJsonArray)
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

sub_struct_end

//------------------------------------------------------------------------------
}
#endif
