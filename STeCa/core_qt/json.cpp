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

#include "json.hpp"
#include <c2/c/str.h>
#include <c2/c/num.h>
#include <c2/inc/c_cpp>
#include <QStringList>
#undef NAN

namespace core_qt {
//------------------------------------------------------------------------------

namespace json_key {
static qstrc
  I("i"), J("j"), X("x"), Y("y"), MIN("min"), MAX("max"),
  PARAMS("parameters"), TYPE("type"), FUN("f%1"), SUM("sum"),
  VALUE("value"), RANGE("range"), COUNT("count"),
  PEAK("guessed peak"), FWHM("guessed fwhm");
}

//------------------------------------------------------------------------------

static JsonObj toJson(c::ij::rc ij) {
  return JsonObj()
    .saveInt(json_key::I, ij.i)
    .saveInt(json_key::J, ij.j);
}

static c::ij toIJ(JsonObj::rc obj) may_err {
  return c::ij(
    obj.loadInt(json_key::I),
    obj.loadInt(json_key::J)
  );
}

TEST("IJ::json",
  c::ij ij(-1,2), ij1(toIJ(toJson(ij)));
  CHECK_EQ(ij, ij1);
)

static JsonObj toJson(c::xy::rc xy) {
  return JsonObj()
    .saveReal(json_key::X, xy.x)
    .saveReal(json_key::Y, xy.y);
}

static c::xy toXY(JsonObj::rc obj) may_err {
  return c::xy(
    obj.loadInt(json_key::X),
    obj.loadInt(json_key::Y)
  );
}

TEST("XY::json",
  c::xy xy(-1,2), xy1(toXY(toJson(xy)));
  CHECK_EQ(xy, xy1);
)

//------------------------------------------------------------------------------

using core::Range;
using core::Ranges;

static JsonObj toJson(Range::rc rge) {
  return JsonObj()
    .saveReal(json_key::MIN, rge.min)
    .saveReal(json_key::MAX, rge.max);
}

static Range toRange(JsonObj::rc obj) may_err {
  return Range(
    obj.loadReal(json_key::MIN),
    obj.loadReal(json_key::MAX)
  );
}

TEST("Range::json",
  Range r(-1,2), r1(toRange(toJson(r)));
  CHECK_EQ(r, r1);
)

static JsonArr toJson(Ranges::rc rs) {
  JsonArr arr;
  for_i (rs.size())
    arr.append(toJson(rs.at(i)));
  return arr;
}

static Ranges toRanges(JsonArr::rc arr) may_err {
  Ranges rs;
  for_i (arr.count())
    rs.add(toRange(arr.objAt(i)));
  return rs;
}

TEST("Ranges::json",
  Ranges rs;
  rs.add(Range());
  rs.add(Range(9));
  rs.add(Range(-3, -2));
  rs.add(Range::inf());

  Ranges rs1(toRanges(toJson(rs)));
  CHECK_EQ(rs, rs1);
)

//------------------------------------------------------------------------------
// TODO impl from fun.cpp

using core::Par;
using core::Fun;
using core::SimpleFun;
using core::SumFuns;

static JsonObj toJson(Par::rc par) {
  return JsonObj()
    .saveReal(json_key::VALUE, par.val);
}

static Par toPar(JsonObj::rc obj) may_err {
  return Par(obj.loadReal(json_key::VALUE), 0);
}

static JsonObj toJson(Fun::rc f);

static JsonObj toJson(SimpleFun::rc f) {
  JsonArr arr;
  for_i (f.parCount())
    arr.append(toJson(f.parAt(i)));
  return JsonObj().saveArr(json_key::PARAMS, arr);
}

static JsonObj toJson(SumFuns::rc f) {
  JsonObj obj;
  obj.saveStr(json_key::TYPE, json_key::SUM);

  uint funCount = c::to_uint(f.funs.size());
  obj.saveUint(json_key::COUNT, funCount);

  for_i (funCount)
    obj.saveObj(json_key::FUN.arg(i+1), toJson(*f.funs.at(i).ptr()));

  return obj;
}

static JsonObj toJson(Fun::rc f) {
  if (dynamic_cast<SimpleFun const*>(&f))
    return toJson(static_cast<SimpleFun const&>(f));
  if (dynamic_cast<SumFuns const*>(&f))
    return toJson(static_cast<SumFuns const&>(f));

  EXPECT (false)
  return JsonObj();
}

static c::own<Fun> loadFun(JsonObj::rc obj);

static void loadSimpleFun(SimpleFun& f, JsonObj::rc obj) {
  JsonArr arr = obj.loadArr(json_key::PARAMS);
  for_i (arr.count())
    f.add(toPar(arr.objAt(i)));
}

static void loadSumFuns(SumFuns& f, JsonObj::rc obj) may_err {
  EXPECT (f.funs.isEmpty()) // cannot load twice

  for_i (obj.loadUint(json_key::COUNT))
    f.add(loadFun(obj.loadObj(json_key::FUN.arg(i + 1))));
}

static c::own<Fun> loadFun(JsonObj::rc obj) may_err {
  auto f = scope(Fun::make(toStr(obj.loadStr(json_key::TYPE))));
  if (dynamic_cast<SimpleFun*>(f.ptr()))
    loadSimpleFun(static_cast<SimpleFun&>(*f), obj);
  else if (dynamic_cast<SumFuns*>(f.ptr()))
    loadSumFuns(static_cast<SumFuns&>(*f), obj);

  return f.take().justOwn();
}

TEST("Par::json",
  Par par(6,0), par1(toPar(toJson(par)));
  CHECK_EQ(par.val,  par1.val);
  CHECK_EQ(par1.err, 0);
)

//------------------------------------------------------------------------------

JsonObj::JsonObj() {}

JsonObj::JsonObj(QJsonObject const& obj) : base(obj) {}

#define ERR(qmsg)     c::err(toStr(qmsg));
#define KEY_ERR(msg)  ERR(key + ": " + msg)

#define LOAD_DEF(type)      value(key).isUndefined() ? def : load##type(key)
#define RET_LOAD_DEF(type)  return LOAD_DEF(type);

JsonObj& JsonObj::saveObj(qstrc key, JsonObj::rc obj) {
  insert(key, obj);
  return *this;
}

JsonObj JsonObj::loadObj(qstrc key, bool defEmpty) const may_err {
  auto val = value(key);

  switch (val.type()) {
  case QJsonValue::Object:
    return val.toObject();
  case QJsonValue::Undefined:
    if (defEmpty)
      return JsonObj();
    _if_clang_([[clang::fallthrough]];)
  default:
    KEY_ERR("not an object");
  }
}

JsonObj& JsonObj::saveArr(qstrc key, JsonArr::rc arr) {
  insert(key, arr.base_rc());
  return *this;
}

JsonArr JsonObj::loadArr(qstrc key, bool defEmpty) const may_err {
  auto val = value(key);

  switch (val.type()) {
  case QJsonValue::Array:
    return val.toArray();
  case QJsonValue::Undefined:
    if (defEmpty)
      return JsonArr();
    _if_clang_([[clang::fallthrough]];)
  default:
    KEY_ERR("not an array")
  }
}

JsonObj& JsonObj::saveInt(qstrc key, int num) {
  insert(key, num);
  return *this;
}

int JsonObj::loadInt(qstrc key) const may_err {
  auto val = value(key);

  switch (val.type()) {
  case QJsonValue::Double:
    return qRound(val.toDouble());
  default:
    KEY_ERR("bad number format")
  }
}

int JsonObj::loadInt(qstrc key, int def) const may_err {
  RET_LOAD_DEF(Int)
}

JsonObj& JsonObj::saveUint(qstrc key, uint num) {
  return saveInt(key, c::to_i(num));
}

uint JsonObj::loadUint(qstrc key) const may_err {
  int num = loadInt(key);
  if (num < 0)
    KEY_ERR("bad number format")
  return c::to_u(num);
}

uint JsonObj::loadUint(qstrc key, uint def) const may_err {
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

real JsonObj::loadReal(qstrc key) const may_err {
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

real JsonObj::loadReal(qstrc key, real def) const may_err {
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

bool JsonObj::loadBool(qstrc key) const may_err {
  auto val = value(key);

  switch (val.type()) {
  case QJsonValue::Bool:
    return val.toBool();
  default:
    KEY_ERR("not a boolean")
  }
}

bool JsonObj::loadBool(qstrc key, bool def) const may_err {
  RET_LOAD_DEF(Bool)
}

JsonObj& JsonObj::saveStr(qstrc key, qstrc s) {
  insert(key, s);
  return *this;
}

qstr JsonObj::loadStr(qstrc key) const may_err {
  auto val = value(key);

  switch (val.type()) {
  case QJsonValue::String:
    return val.toString();
  default:
    KEY_ERR("not a string")
  }
}

qstr JsonObj::loadStr(qstrc key, qstrc def) const may_err {
  RET_LOAD_DEF(Str)
}

JsonObj& JsonObj::saveRange(qstrc key, Range::rc range) {
  insert(key, toJson(range));
  return *this;
}

Range JsonObj::loadRange(qstrc key) const may_err {
  return toRange(loadObj(key));
}

JsonObj& JsonObj::saveRanges(qstrc key, Ranges::rc ranges) {
  saveArr(key, toJson(ranges));
  return *this;
}

Ranges JsonObj::loadRanges(qstrc key) const may_err {
  return toRanges(loadArr(key));
}

JsonObj& JsonObj::saveIJ(qstrc key, c::ij::rc ij) {
  insert(key, toJson(ij));
  return *this;
}

c::ij JsonObj::loadIJ(qstrc key) const may_err {
  return toIJ(loadObj(key));
}

JsonObj& JsonObj::saveXY(qstrc key, c::xy::rc xy) {
  insert(key, toJson(xy));
  return *this;
}

c::xy JsonObj::loadXY(qstrc key) const may_err {
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
}
// eof
