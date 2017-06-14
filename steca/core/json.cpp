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
#include <dev_lib/inc/defs_cpp.h>
#include <dev_lib/inc/ptr.h>
#include <qt_lib/str_inc.h>

namespace core { namespace key {

str const
  I("i"), J("j"), X("x"), Y("y"), MIN("min"), MAX("max"),
  PARAMS("parameters"), TYPE("type"), FUN("f"), SUM("sum"),
  VALUE("value"), RANGE("range"), COUNT("count"),
  PEAK("guessed peak"), FWHM("guessed fwhm"),
  POLYNOM("polynom"), RAW("Raw"),
  GAUSSIAN("Gaussian"), LORENTZIAN("Lorentzian"),
  PSEUDOVOIGT1("PseudoVoigt1"), PSEUDOVOIGT2("PseudoVoigt2");

}}

namespace core_qt {
//------------------------------------------------------------------------------

static JsonObj toJson(l::ij::rc ij) {
  return JsonObj()
    .saveInt(core::key::I, ij.i)
    .saveInt(core::key::J, ij.j);
}

static l::ij toIJ(JsonObj::rc obj) may_err {
  return l::ij(
    obj.loadInt(core::key::I),
    obj.loadInt(core::key::J)
  );
}

TEST_("IJ::json",
  l::ij ij(-1,2), ij1(toIJ(toJson(ij)));
  CHECK_EQ(ij, ij1);
)

static JsonObj toJson(l::xy::rc xy) {
  return JsonObj()
    .saveReal(core::key::X, xy.x)
    .saveReal(core::key::Y, xy.y);
}

static l::xy toXY(JsonObj::rc obj) may_err {
  return l::xy(
    obj.loadInt(core::key::X),
    obj.loadInt(core::key::Y)
  );
}

TEST_("XY::json",
  l::xy xy(-1,2), xy1(toXY(toJson(xy)));
  CHECK_EQ(xy, xy1);
)

//------------------------------------------------------------------------------

using core::Range;
using core::Ranges;

static JsonObj toJson(Range::rc rge) {
  return JsonObj()
    .saveReal(core::key::MIN, rge.min)
    .saveReal(core::key::MAX, rge.max);
}

static Range toRange(JsonObj::rc obj) may_err {
  return Range(
    obj.loadReal(core::key::MIN),
    obj.loadReal(core::key::MAX)
  );
}

TEST_("Range::json",
  Range r(-1,2), r1(toRange(toJson(r)));
  CHECK_EQ(r, r1);
)

static JsonArr toJson(Ranges::rc rs) {
  JsonArr arr;
  for_i_(rs.size())
    arr.append(toJson(rs.at(i)));
  return arr;
}

static Ranges toRanges(JsonArr::rc arr) may_err {
  Ranges rs;
  for_i_(arr.count())
    rs.add(toRange(arr.objAt(i)));
  return rs;
}

TEST_("Ranges::json",
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
    .saveReal(core::key::VALUE, par.val);
}

static Par toPar(JsonObj::rc obj) may_err {
  return Par(obj.loadReal(core::key::VALUE), 0);
}

static JsonObj toJson(Fun::rc f);

static JsonObj toJson(SimpleFun::rc f) {
  JsonArr arr;
  for_i_(f.parCount())
    arr.append(toJson(f.parAt(i)));
  return JsonObj().saveArr(core::key::PARAMS, arr);
}

static JsonObj toJson(SumFuns::rc f) {
  JsonObj obj;
  obj.saveStr(core::key::TYPE, core::key::SUM);

  uint funCount = l::to_uint(f.funs.size());
  obj.saveUint(core::key::COUNT, funCount);

  for_i_(funCount)
    obj.saveObj(CAT(core::key::FUN, (i+1)), toJson(*f.funs.at(i)));

  return obj;
}

static JsonObj toJson(Fun::rc f) {
  if (dynamic_cast<SimpleFun const*>(&f))
    return toJson(static_cast<SimpleFun const&>(f));
  if (dynamic_cast<SumFuns const*>(&f))
    return toJson(static_cast<SumFuns const&>(f));

  EXPECT_(false)
  return JsonObj();
}

static l::own<Fun> loadFun(JsonObj::rc obj);

static void loadSimpleFun(SimpleFun& f, JsonObj::rc obj) {
  JsonArr arr = obj.loadArr(core::key::PARAMS);
  for_i_(arr.count())
    f.add(toPar(arr.objAt(i)));
}

static void loadSumFuns(SumFuns& f, JsonObj::rc obj) may_err {
  EXPECT_(f.funs.isEmpty()) // cannot load twice

  for_i_(obj.loadUint(core::key::COUNT))
    f.add(loadFun(obj.loadObj(CAT(core::key::FUN, (i+1)))));
}

static l::own<Fun> loadFun(JsonObj::rc obj) may_err {
  auto f = l::scope(Fun::make(obj.loadStr(core::key::TYPE)));
  if (dynamic_cast<SimpleFun*>(f.ptr()))
    loadSimpleFun(static_cast<SimpleFun&>(*f), obj);
  else if (dynamic_cast<SumFuns*>(f.ptr()))
    loadSumFuns(static_cast<SumFuns&>(*f), obj);

  return f.take().justOwn();
}

TEST_("Par::json",
  Par par(6,0), par1(toPar(toJson(par)));
  CHECK_EQ(par.val,  par1.val);
  CHECK_EQ(par1.err, 0);
)

//------------------------------------------------------------------------------

JsonObj::JsonObj() {}

JsonObj::JsonObj(QJsonObject const& obj) : base(obj) {}

#define LOAD_DEF(type)      value(key).isUndefined() ? def : load##type(key)
#define RET_LOAD_DEF(type)  return LOAD_DEF(type);

#define key_err(msg) l::err(key + ": " +  msg)

JsonObj& JsonObj::saveObj(strc key, JsonObj::rc obj) {
  insert(key, obj);
  return *this;
}

JsonObj JsonObj::loadObj(strc key, bool defEmpty) const may_err {
  auto val = value(key);

  switch (val.type()) {
  case QJsonValue::Object:
    return val.toObject();
  case QJsonValue::Undefined:
    if (defEmpty)
      return JsonObj();
    _if_clang_([[clang::fallthrough]];)
  default:
    key_err("not an object");
  }
}

JsonObj& JsonObj::saveArr(strc key, JsonArr::rc arr) {
  insert(key, arr.base_rc());
  return *this;
}

JsonArr JsonObj::loadArr(strc key, bool defEmpty) const may_err {
  auto val = value(key);

  switch (val.type()) {
  case QJsonValue::Array:
    return val.toArray();
  case QJsonValue::Undefined:
    if (defEmpty)
      return JsonArr();
    _if_clang_([[clang::fallthrough]];)
  default:
    key_err("not an array");
  }
}

JsonObj& JsonObj::saveInt(strc key, int num) {
  insert(key, num);
  return *this;
}

int JsonObj::loadInt(strc key) const may_err {
  auto val = value(key);

  switch (val.type()) {
  case QJsonValue::Double:
    return qRound(val.toDouble());
  default:
    key_err("bad number format");
  }
}

int JsonObj::loadInt(strc key, int def) const may_err {
  RET_LOAD_DEF(Int)
}

JsonObj& JsonObj::saveUint(strc key, uint num) {
  return saveInt(key, l::to_i(num));
}

uint JsonObj::loadUint(strc key) const may_err {
  int num = loadInt(key);
  if (num < 0)
    key_err("bad number format");
  return l::to_u(num);
}

uint JsonObj::loadUint(strc key, uint def) const may_err {
  RET_LOAD_DEF(Uint)
}

JsonObj& JsonObj::savePint(strc key, pint num) {
  return saveUint(key, num);
}

l::pint JsonObj::loadPint(strc key) const {
  uint num = loadUint(key);
  if (num < 1)
    key_err("expecting a positive integer");
  return pint(num);
}

l::pint JsonObj::loadPint(strc key, uint def) const {
  return pint(LOAD_DEF(Pint));
}

static QString INF_P("+inf"), INF_M("-inf");

JsonObj& JsonObj::saveReal(strc key, real num) {
  if (l::isnan(num)) {
    // do save nothing for nans
  } else if (l::isinf(num)) {
    insert(key, num < 0 ? INF_M : INF_P);
  } else {
    insert(key, num);
  }

  return *this;
}

real JsonObj::loadReal(strc key) const may_err {
  auto val = value(key);

  switch (val.type()) {
  case QJsonValue::Undefined:
    return l::flt_nan;          // not present means nan
  case QJsonValue::String: {  // infinities stored as strings
    auto s = val.toString();
    if (INF_P == s)
      return +l::flt_inf;
    if (INF_M == s)
      return -l::flt_inf;
    key_err("bad number format");
  }
  default:
    return val.toDouble();
  }
}

real JsonObj::loadReal(strc key, real def) const may_err {
  RET_LOAD_DEF(Real)
}

JsonObj& JsonObj::savePeal(strc key, peal num) {
  return saveReal(key, num);
}

l::peal JsonObj::loadPeal(strc key) const {
  real num = loadReal(key);
  if (num <= 0)
    key_err("expecting a positive real");
  return peal(num);
}

l::peal JsonObj::loadPeal(strc key, peal def) const {
  RET_LOAD_DEF(Peal)
}

JsonObj& JsonObj::saveBool(strc key, bool b) {
  insert(key, b);
  return *this;
}

bool JsonObj::loadBool(strc key) const may_err {
  auto val = value(key);

  switch (val.type()) {
  case QJsonValue::Bool:
    return val.toBool();
  default:
    key_err("not a boolean");
  }
}

bool JsonObj::loadBool(strc key, bool def) const may_err {
  RET_LOAD_DEF(Bool)
}

JsonObj& JsonObj::saveStr(strc key, strc s) {
  insert(key, s);
  return *this;
}

str JsonObj::loadStr(strc key) const may_err {
  auto val = value(key);

  switch (val.type()) {
  case QJsonValue::String:
    return l_qt::fromQt(val.toString());
  default:
    key_err("not a string");
  }
}

str JsonObj::loadStr(strc key, strc def) const may_err {
  RET_LOAD_DEF(Str)
}

JsonObj& JsonObj::saveRange(strc key, Range::rc range) {
  insert(key, toJson(range));
  return *this;
}

Range JsonObj::loadRange(strc key) const may_err {
  return toRange(loadObj(key));
}

JsonObj& JsonObj::saveRanges(strc key, Ranges::rc ranges) {
  saveArr(key, toJson(ranges));
  return *this;
}

Ranges JsonObj::loadRanges(strc key) const may_err {
  return toRanges(loadArr(key));
}

JsonObj& JsonObj::saveIJ(strc key, l::ij::rc ij) {
  insert(key, toJson(ij));
  return *this;
}

l::ij JsonObj::loadIJ(strc key) const may_err {
  return toIJ(loadObj(key));
}

JsonObj& JsonObj::saveXY(strc key, l::xy::rc xy) {
  insert(key, toJson(xy));
  return *this;
}

l::xy JsonObj::loadXY(strc key) const may_err {
  return toXY(loadObj(key));
}

JsonObj& JsonObj::operator+=(JsonObj::rc that) {
  for (auto& key : that.keys())
    base::insert(key, that[key]);
  return *this;
}

JsonObj JsonObj::operator+(JsonObj::rc that) const {
  return JsonObj(*this) += that;
}

JsonObj::iterator JsonObj::insert(strc key, strc val) {
  return base::insert(l_qt::toQt(key), l_qt::toQt(val));
}

JsonObj::iterator JsonObj::insert(strc key, QJsonValue const& val) {
  return base::insert(l_qt::toQt(key), val);
}

QJsonValue JsonObj::value(strc key) const {
  return base::value(l_qt::toQt(key));
}

//------------------------------------------------------------------------------

JsonArr::JsonArr() {
}

JsonArr::JsonArr(QJsonArray const& array) : base(array) {}

void JsonArr::append(JsonObj::rc obj) {
  base::append(obj.base_rc());
}

uint JsonArr::count() const {
  return l::to_u(base::count());
}

JsonObj JsonArr::objAt(uint i) const {
  auto obj = base::at(l::to_i(i));
  if (QJsonValue::Object != obj.type())
    l::err(CAT("not an object at: ", i));
  return base::at(l::to_i(i)).toObject();
}

//------------------------------------------------------------------------------
}
// eof
