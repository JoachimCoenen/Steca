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

#include "json.h"
#include <c/c/cpp>

namespace core {
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
}
// eof
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

#include "json.h"

#include "def/def_exc.h"
#include "typ/typ_ij.h"
#include "typ/typ_range.h"
#include "typ/typ_str.h"
#include "typ/typ_xy.h"
#include <QStringList>

namespace json_key {
str const
  I("i"), J("j"), X("x"), Y("y"), MIN("min"), MAX("max"),
  PARAMS("parameters"), TYPE("type"), FUN("f%1"),
  VALUE("value"), RANGE("range"), COUNT("count"),
  PEAK("guessed peak"), FWHM("guessed fwhm");
}

namespace typ {
//------------------------------------------------------------------------------

JsonObj::JsonObj() {
}

JsonObj::JsonObj(QJsonObject const& obj) : super(obj) {}

JsonObj& JsonObj::saveObj(qstrc key, JsonObj::rc obj) {
  insert(key, obj.sup());
  return *this;
}

JsonObj JsonObj::loadObj(qstrc key, bool defEmpty) const may_exc {
  auto val = value(key);

  switch (val.type()) {
  case QJsonValue::Object:
    return val.toObject();
  case QJsonValue::Undefined:
    if (defEmpty)
      return JsonObj();
    // fallthrough
  default:
    THROW(key + ": not an object");
  }
}

JsonObj& JsonObj::saveArr(qstrc key, JsonArr::rc arr) {
  insert(key, arr.sup());
  return *this;
}

JsonArr JsonObj::loadArr(qstrc key, bool defEmpty) const may_exc {
  auto val = value(key);

  switch (val.type()) {
  case QJsonValue::Array:
    return val.toArray();
  case QJsonValue::Undefined:
    if (defEmpty)
      return JsonArr();
    // fall through
  default:
    THROW(key + ": not an array");
  }
}

JsonObj& JsonObj::saveInt(qstrc key, int num) {
  insert(key, num);
  return *this;
}

int JsonObj::loadInt(qstrc key) const may_exc {
  auto val = value(key);

  switch (val.type()) {
  case QJsonValue::Double:
    return qRound(val.toDouble());
  default:
    THROW(key + ": bad number format");
  }
}

#define LOAD_DEF(type) \
  value(key).isUndefined() ? def : load##type(key)

#define RET_LOAD_DEF(type) \
  return LOAD_DEF(type);

int JsonObj::loadInt(qstrc key, int def) const may_exc {
  RET_LOAD_DEF(Int)
}

JsonObj& JsonObj::saveUint(qstrc key, uint num) {
  return saveInt(key, to_i(num));
}

uint JsonObj::loadUint(qstrc key) const may_exc {
  int num = loadInt(key);
  if (num < 0)
    THROW(key + ": bad number format");
  return to_u(num);
}

uint JsonObj::loadUint(qstrc key, uint def) const may_exc {
  RET_LOAD_DEF(Uint)
}

JsonObj& JsonObj::savePint(qstrc key, pint num) {
  return saveUint(key, num);
}

pint JsonObj::loadPint(qstrc key) const {
  uint num = loadUint(key);
  RUNTIME_CHECK(num > 0, "expecting positive number");
  return pint(num);
}

pint JsonObj::loadPint(qstrc key, uint def) const {
  return pint(LOAD_DEF(Pint));
}

static str const INF_P("+inf"), INF_M("-inf");

JsonObj& JsonObj::saveReal(qstrc key, real num) {
  if (isnan(num)) {
    // do not save anything for NaNs
  } else if (c::isinf(num)) {
    insert(key, num < 0 ? INF_M : INF_P);
  } else {
    insert(key, num);
  }

  return *this;
}

real JsonObj::loadReal(qstrc key) const may_exc {
  auto val = value(key);

  switch (val.type()) {
  case QJsonValue::Undefined:
    return c::NAN;           // not present means not a number
  case QJsonValue::String: {  // infinities stored as strings
    auto s = val.toString();
    if (INF_P == s)
      return +INF;
    if (INF_M == s)
      return -INF;
    THROW(key + ": bad number format");
  }
  default:
    return val.toDouble();
  }
}

real JsonObj::loadReal(qstrc key, real def) const may_exc {
  RET_LOAD_DEF(Qreal)
}

JsonObj& JsonObj::savePreal(qstrc key, preal num) {
  return saveReal(key, num);
}

preal JsonObj::loadPreal(qstrc key) const {
  real num = loadReal(key);
  RUNTIME_CHECK(num >= 0, "expecting positive number");
  return preal(num);
}

preal JsonObj::loadPreal(qstrc key, preal def) const {
  RET_LOAD_DEF(Preal)
}

JsonObj& JsonObj::saveBool(qstrc key, bool b) {
  insert(key, b);
  return *this;
}

bool JsonObj::loadBool(qstrc key) const may_exc {
  auto val = value(key);

  switch (val.type()) {
  case QJsonValue::Bool:
    return val.toBool();
  default:
    THROW(key + ": not a boolean");
  }
}

bool JsonObj::loadBool(qstrc key, bool def) const may_exc {
  RET_LOAD_DEF(Bool)
}

JsonObj& JsonObj::saveString(qstrc key, qstrc s) {
  insert(key, s);
  return *this;
}

str JsonObj::loadString(qstrc key) const may_exc {
  auto val = value(key);

  switch (val.type()) {
  case QJsonValue::String:
    return val.toString();
  default:
    THROW(key + ": not a string");
  }
}

str JsonObj::loadString(qstrc key, qstrc def) const may_exc {
  RET_LOAD_DEF(String)
}

JsonObj& JsonObj::saveRange(qstrc key, Range::rc range) {
  insert(key, range.saveJson());
  return *this;
}

Range JsonObj::loadRange(qstrc key) const may_exc {
  Range range;
  range.loadJson(loadObj(key));
  return range;
}

JsonObj& JsonObj::saveIJ(qstrc key, IJ::rc ij) {
  insert(key, ij.saveJson());
  return *this;
}

IJ JsonObj::loadIJ(qstrc key) const may_exc {
  IJ ij;
  ij.loadJson(loadObj(key));
  return ij;
}

JsonObj& JsonObj::saveXY(qstrc key, XY::rc xy) {
  insert(key, xy.saveJson());
  return *this;
}

XY JsonObj::loadXY(qstrc key) const may_exc {
  XY xy;
  xy.loadJson(loadObj(key));
  return xy;
}

JsonObj& JsonObj::operator+=(JsonObj::rc that) {
  for (auto& key : that.keys())
    insert(key, that[key]);
  return *this;
}

JsonObj JsonObj::operator+(JsonObj::rc that) const {
  return JsonObj(*this) += that;
}

//------------------------------------------------------------------------------

JsonArr::JsonArr() {
}

JsonArr::JsonArr(QJsonArray const& array) : super(array) {}

void JsonArr::append(JsonObj::rc obj) {
  super::append(obj.sup());
}

uint JsonArr::count() const {
  return to_u(super::count());
}

JsonObj JsonArr::objAt(uint i) const {
  auto obj = super::at(to_i(i));
  RUNTIME_CHECK(QJsonValue::Object == obj.type(),
                "not an object at " + str::number(i));
  return super::at(to_i(i)).toObject();
}

//------------------------------------------------------------------------------
}
// eof
IJ
JsonObj IJ::saveJson() const {
  return JsonObj().saveInt(json_key::I, i).saveInt(json_key::J, j);
}

void IJ::loadJson(JsonObj::rc obj) may_exc {
  i = obj.loadInt(json_key::I);
  j = obj.loadInt(json_key::J);
}

TEST("IJ::json", ({
  IJ ij(-1,2), ij1;
  ij1.loadJson(ij.saveJson());
  CHECK_EQ(ij, ij1);
})
XY
     JsonObj XY::saveJson() const {
       return JsonObj().saveReal(json_key::X, x).saveReal(json_key::Y, y);
     }

     void XY::loadJson(JsonObj::rc obj) may_exc {
       x = obj.loadReal(json_key::X);
       y = obj.loadReal(json_key::Y);
     }

     TEST("XY::json", ({
       XY xy(-1,2), xy1;
       xy1.loadJson(xy.saveJson());
       CHECK_EQ(xy, xy1);
     })

          JsonObj Range::saveJson() const {
            return JsonObj().saveReal(json_key::MIN, min).saveReal(json_key::MAX, max);
          }

          void Range::loadJson(JsonObj::rc obj) may_exc {
            min = obj.loadReal(json_key::MIN);
            max = obj.loadReal(json_key::MAX);
          }

          TEST("Range::json", ({
            Range r(-1,2), r1;
            r1.loadJson(r.saveJson());
            CHECK_EQ(r, r1);
          })

               JsonArr Ranges::saveJson() const {
                 JsonArr arr;

                 for (auto& range : ranges_)
                   arr.append(range.saveJson());

                 return arr;
               }

               void Ranges::loadJson(JsonArr::rc arr) may_exc {
                 for_i (arr.count()) {
                   Range range;
                   range.loadJson(arr.objAt(i));
                   ranges_.append(range);
                 }
               }


               TEST("Ranges::json", ({
                 Ranges rs, rs1;
                 rs.add(Range());
                 rs.add(Range(9));
                 rs.add(Range(-3, -2));
                 rs.add(Range::infinite());

                 rs1.loadJson(rs.saveJson());
                 CHECK(RANGES_EQ(rs, rs1));
               })
