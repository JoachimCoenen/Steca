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

#include <c2/inc/c_c.h>
#include <c2/c/num.h>
#include <c2/c/ptr.h>

#include <c2/cpp/exc.hpp>

#include <c2/c/ij.h>
#include <c2/c/xy.h>
#include <core/typ/range.hpp>
#include <core/typ/fun.hpp>

#include <QJsonArray>
#include <QJsonObject>

// TODO perhaps move all this, and io, out of core to io
namespace core_qt {
//------------------------------------------------------------------------------

struct JsonArr;

dcl_struct_reimpl_(JsonObj, QJsonObject)
  using strc = c::strc;

  using pint = c::pint;
  using peal = c::peal;

  JsonObj();
  JsonObj(QJsonObject const&);

  // plain types
  JsonObj& saveObj(strc key, JsonObj const&);
  JsonObj  loadObj(strc key, bool defEmpty=false) const may_err;

  JsonObj& saveArr(strc key, JsonArr const&);
  JsonArr  loadArr(strc key, bool defEmpty=false) const may_err;

  JsonObj& saveInt(strc key, int);
  int      loadInt(strc key)               const may_err;
  int      loadInt(strc key, int def)      const may_err;

  JsonObj& saveUint(strc key, uint);
  uint     loadUint(strc key)              const may_err;
  uint     loadUint(strc key, uint def)    const may_err;

  JsonObj& savePint(strc key, pint);
  pint     loadPint(strc key)              const may_err;
  pint     loadPint(strc key, uint def)    const may_err;

  JsonObj& saveReal(strc key, real);
  real     loadReal(strc key)              const may_err;
  real     loadReal(strc key, real def)    const may_err;

  JsonObj& savePeal(strc key, peal);
  peal     loadPeal(strc key)             const may_err;
  peal     loadPeal(strc key, peal def)   const may_err;

  JsonObj& saveBool(strc key, bool);
  bool     loadBool(strc key)              const may_err;
  bool     loadBool(strc key, bool def)    const may_err;

  JsonObj& saveStr(strc key, strc);
  c::str   loadStr(strc key)               const may_err;
  c::str   loadStr(strc key, strc def)    const may_err;

  // more custom types
  JsonObj& saveIJ(strc key, c::ij::rc);
  c::ij    loadIJ(strc key)                const may_err;

  JsonObj& saveXY(strc key, c::xy::rc);
  c::xy    loadXY(strc key)                const may_err;

  JsonObj&     saveRange(strc key, core::Range::rc);
  core::Range  loadRange(strc key)         const may_err;

  JsonObj&     saveRanges(strc key, core::Ranges::rc);
  core::Ranges loadRanges(strc key)        const may_err;

  JsonObj& operator+= (rc);
  JsonObj  operator+  (rc) const;

protected_
  iterator insert(strc, strc);
  iterator insert(strc, QJsonValue const&);

  QJsonValue value(strc) const;
dcl_end

//------------------------------------------------------------------------------

dcl_struct_reimpl_(JsonArr, QJsonArray)
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

dcl_end

//------------------------------------------------------------------------------
}
#endif
