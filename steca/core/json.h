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

#include <app_lib/inc/defs_h.h>
#include <app_lib/inc/num.h>
#include <app_lib/typ/ij.h>
#include <app_lib/typ/xy.h>

#include <core/typ/range.h>
#include <core/typ/fun.h>

#include <QJsonArray>
#include <QJsonObject>

// TODO perhaps move all this, and io, out of core to io
namespace core_qt {
//------------------------------------------------------------------------------

struct JsonArr;

dcl_reimpl_(JsonObj, QJsonObject)
  using pint = l::pint;
  using peal = l::peal;

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
  str   loadStr(strc key)               const may_err;
  str   loadStr(strc key, strc def)    const may_err;

  // more custom types
  JsonObj& saveIJ(strc key, l::ij::rc);
  l::ij    loadIJ(strc key)                const may_err;

  JsonObj& saveXY(strc key, l::xy::rc);
  l::xy    loadXY(strc key)                const may_err;

  JsonObj&     saveRange(strc key, core::Range::rc);
  core::Range  loadRange(strc key)         const may_err;

  JsonObj&     saveRanges(strc key, core::Ranges::rc);
  core::Ranges loadRanges(strc key)        const may_err;

  JsonObj& operator+= (rc);
  JsonObj  operator+  (rc) const;

protected:
  iterator insert(strc, strc);
  iterator insert(strc, QJsonValue const&);

  QJsonValue value(strc) const;
dcl_end

//------------------------------------------------------------------------------

dcl_reimpl_(JsonArr, QJsonArray)
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
