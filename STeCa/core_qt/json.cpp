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
#include <c/c/lib/str.h>
#include <QStringList>

namespace core {
//------------------------------------------------------------------------------

namespace json_key {
static qstrc
  I("i"), J("j"), X("x"), Y("y"), MIN("min"), MAX("max"),
  PARAMS("parameters"), TYPE("type"), FUN("f%1"),
  VALUE("value"), RANGE("range"), COUNT("count"),
  PEAK("guessed peak"), FWHM("guessed fwhm");
}

JsonObj::JsonObj() {}

JsonObj::JsonObj(QJsonObject const& obj) : base(obj) {}

JsonObj& JsonObj::saveObj(qstrc key, JsonObj::rc obj) {
  insert(key, obj);
  return *this;
}

#define ERR(msg)     c::qt::err(msg);
#define KEY_ERR(msg) ERR(key + ": " + msg)

JsonObj JsonObj::loadObj(qstrc key, bool defEmpty) const may_exc {
  auto val = value(key);

  switch (val.type()) {
  case QJsonValue::Object:
    return val.toObject();
  case QJsonValue::Undefined:
    if (defEmpty)
      return JsonObj();
    [[clang::fallthrough]];
  default:
    KEY_ERR("not an object");
  }
}

JsonObj& JsonObj::saveArr(qstrc key, JsonArr::rc arr) {
  insert(key, arr.base_rc());
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
    [[clang::fallthrough]];
  default:
    KEY_ERR("not an array")
  }
}

#define LOAD_DEF(type) \
  value(key).isUndefined() ? def : load##type(key)

#define RET_LOAD_DEF(type) \
  return LOAD_DEF(type);

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
    KEY_ERR("bad number format")
  }
}

int JsonObj::loadInt(qstrc key, int def) const may_exc {
  RET_LOAD_DEF(Int)
}

JsonObj& JsonObj::saveUint(qstrc key, uint num) {
  return saveInt(key, c::to_i(num));
}

uint JsonObj::loadUint(qstrc key) const may_exc {
  int num = loadInt(key);
  if (num < 0)
    KEY_ERR("bad number format")
  return c::to_u(num);
}

uint JsonObj::loadUint(qstrc key, uint def) const may_exc {
  RET_LOAD_DEF(Uint)
}

JsonObj& JsonObj::savePint(qstrc key, pint num) {
  return saveUint(key, num);
}

c::pint JsonObj::loadPint(qstrc key) const {
  uint num = loadUint(key);
  if (num < 1)
    KEY_ERR("expecting a positive integer")
  return pint(num);
}

c::pint JsonObj::loadPint(qstrc key, uint def) const {
  return pint(LOAD_DEF(Pint));
}

static qstrc INF_P("+inf"), INF_M("-inf");

JsonObj& JsonObj::saveReal(qstrc key, real num) {
  if (c::isnan(num)) {
    // do save nothing for NANs
  } else if (c::isinf(num)) {
    insert(key, qstr(num < 0 ? INF_M : INF_P));
  } else {
    insert(key, num);
  }

  return *this;
}

real JsonObj::loadReal(qstrc key) const may_exc {
  auto val = value(key);

  switch (val.type()) {
  case QJsonValue::Undefined:
    return c::NAN;            // not present means a NAN
  case QJsonValue::String: {  // infinities stored as strings
    auto s = val.toString();
    if (INF_P == s)
      return +c::INF;
    if (INF_M == s)
      return -c::INF;
    KEY_ERR("bad number format")
  }
  default:
    return val.toDouble();
  }
}

real JsonObj::loadReal(qstrc key, real def) const may_exc {
  RET_LOAD_DEF(Real)
}

JsonObj& JsonObj::savePreal(qstrc key, preal num) {
  return saveReal(key, num);
}

c::preal JsonObj::loadPreal(qstrc key) const {
  real num = loadReal(key);
  if (num <= 0)
    KEY_ERR("expecting a positive real")
  return preal(num);
}

c::preal JsonObj::loadPreal(qstrc key, preal def) const {
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
    KEY_ERR("not a boolean")
  }
}

bool JsonObj::loadBool(qstrc key, bool def) const may_exc {
  RET_LOAD_DEF(Bool)
}

JsonObj& JsonObj::saveString(qstrc key, qstrc s) {
  insert(key, s);
  return *this;
}

qstr JsonObj::loadString(qstrc key) const may_exc {
  auto val = value(key);

  switch (val.type()) {
  case QJsonValue::String:
    return val.toString();
  default:
    KEY_ERR("not a string")
  }
}

qstr JsonObj::loadString(qstrc key, qstrc def) const may_exc {
  RET_LOAD_DEF(String)
}

JsonObj& JsonObj::saveRange(qstrc key, Range::rc range) {
  insert(key, toJson(range));
  return *this;
}

Range JsonObj::loadRange(qstrc key) const may_exc {
  return toRange(loadObj(key));
}

JsonObj& JsonObj::saveIJ(qstrc key, IJ::rc ij) {
  insert(key, toJson(ij));
  return *this;
}

IJ JsonObj::loadIJ(qstrc key) const may_exc {
  return toIJ(loadObj(key));
}

JsonObj& JsonObj::saveXY(qstrc key, XY::rc xy) {
  insert(key, toJson(xy));
  return *this;
}

XY JsonObj::loadXY(qstrc key) const may_exc {
  return toXY(loadObj(key));
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

JsonArr::JsonArr(QJsonArray const& array) : base(array) {}

void JsonArr::append(JsonObj::rc obj) {
  base::append(obj.base_rc());
}

uint JsonArr::count() const {
  return c::to_u(base::count());
}

JsonObj JsonArr::objAt(uint i) const {
  auto obj = base::at(c::to_i(i));
  if (QJsonValue::Object != obj.type())
    ERR("not an object at " + qstr::number(i))
  return base::at(c::to_i(i)).toObject();
}

//------------------------------------------------------------------------------

JsonObj toJson(Range::rc range) {
  return JsonObj()
    .saveReal(json_key::MIN, range.min)
    .saveReal(json_key::MAX, range.max);
}

JsonArr toJson(Ranges::rc rs) {
  JsonArr arr;

  for_i (rs.count())
    arr.append(toJson(rs.at(i)));

  return arr;
}

JsonObj toJson(IJ::rc ij) {
  return JsonObj()
    .saveInt(json_key::I, ij.i)
    .saveInt(json_key::J, ij.j);
}

JsonObj toJson(XY::rc xy) {
  return JsonObj()
    .saveReal(json_key::X, xy.x)
    .saveReal(json_key::Y, xy.y);
}

Range toRange(JsonObj::rc obj) may_exc {
  return Range(
    obj.loadReal(json_key::MIN),
    obj.loadReal(json_key::MAX)
  );
}

Ranges toRanges(JsonArr::rc arr) may_exc {
  Ranges res;
  for_i (arr.count())
    res.add(toRange(arr.objAt(i)));
  return res;
}

IJ toIJ(JsonObj::rc obj) may_exc {
  return IJ(
    obj.loadInt(json_key::I),
    obj.loadInt(json_key::J)
  );
}

XY toXY(JsonObj::rc obj) may_exc {
  return XY(
    obj.loadInt(json_key::X),
    obj.loadInt(json_key::Y)
  );
}

#ifdef WITH_TESTS

static bool RANGES_EQ(Ranges::rc rs1, Ranges::rc rs2) {
  if (rs1.count() != rs2.count())
    return false;

  for_i (rs1.count()) {
    if (rs1.at(i) != rs2.at(i))
      return false;
  }

  return true;
}

#endif

TEST("IJ::json", ({
  IJ ij(-1,2), ij1(toIJ(toJson(ij)));
  CHECK_EQ(ij, ij1);
});)

TEST("XY::json", ({
  XY xy(-1,2), xy1(toXY(toJson(xy)));
  CHECK_EQ(xy, xy1);
});)

TEST("Range::json", ({
  Range r(-1,2), r1(toRange(toJson(r)));
  CHECK_EQ(r, r1);
});)

TEST("Ranges::json", ({
  Ranges rs;
  rs.add(Range());
  rs.add(Range(9));
  rs.add(Range(-3, -2));
  rs.add(Range::infinite());

  Ranges rs1(toRanges(toJson(rs)));
  CHECK(RANGES_EQ(rs, rs1));
});)

//------------------------------------------------------------------------------
}
// eof
