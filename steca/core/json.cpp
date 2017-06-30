//#pragma once
////
////
////  Created by ZK on 14-7-4.
////
////

//#include <cassert>
//#include <map>
//#include <vector>
//#include <iostream>
//#include <set>
//#include <list>
//#include <type_traits>
//using namespace std;



//class serialize
//{
//public:
//    class I
//    {
//    public:

//        virtual std::ostream& write( std::ostream& ostream_) = 0;
//        virtual std::istream& read(std::istream& istream_) = 0;

//    };

//    static inline bool LE()
//    {

//        const static  int n = 1;
//        const static  bool le= (* (char *)&n == 1);
//        return le;
//    }

//    static inline bool ReadEndian (istream &istream_)
//    {
//        bool littleEndian = false;
//        istream_.read ((char*)&littleEndian, sizeof (littleEndian));
//        return littleEndian;
//    }

//    static inline void WriteEndian (ostream &ostream_)
//    {
//        bool littleEndian = LE();

//        ostream_.write ((char*)&littleEndian, sizeof (littleEndian));
//    }

//    static inline istream& read_internal (istream& istream_, char* p, uint32_t size)
//    {

//        if (!LE())
//        {
//            //read as little endian
//            for (int i = size - 1; i >= 0; --i)
//            {
//                istream_.read (p+i, 1);
//            }
//        }
//        else
//            istream_.read (p, size);
//        return  istream_;
//    }

//    static inline ostream& write_internal (ostream& ostream_, const char* p, uint32_t size)
//    {

//        if (!LE())
//        {
//            //if big endian .write as little endian
//            for (int i = size-1; i>=0; --i)
//            {
//                ostream_.write (p + i, 1);
//            }
//        }
//        else
//            ostream_.write (p, size);
//        return  ostream_;
//    }

//    static inline istream& read (istream& istream_, I* t_)
//    {
//        return t_->read (istream_);
//    }

//    static inline istream& read (istream& istream_, std::string& string_)
//    {
//        int size = 0;
//        read (istream_, size);
//        string_.resize (size);
//        read_internal (istream_, const_cast<char*> (string_.c_str()), size);
//        return  istream_;
//    }

//    static inline istream& read (istream& istream_, char* str)
//    {
//        int size = 0;
//        read (istream_, size);
//        read_internal (istream_, str, size);
//        return  istream_;
//    }

//    static inline istream& read (istream& istream_, vector<bool>&container)
//    {
//        if (!istream_.good() || istream_.eof())return istream_;

//        int size;
//        container.clear();
//        read (istream_, size);
//        for (int i = 0; i < size; ++i)
//        {
//            bool t;
//            read (istream_, t);
//            container.push_back (t);
//        }
//        assert (istream_.good());
//        return istream_;
//    }

//    static inline ostream& write (ostream& ostream_, I* t_)
//    {
//        return t_->write (ostream_);
//    }

//    static inline ostream& write (ostream& ostream_, const std::string& string_)
//    {
//        uint32_t size = string_.size();
//        write (ostream_, size);
//        write_internal (ostream_, string_.c_str(), string_.size());
//        return ostream_;
//    }

//    static inline ostream& write (ostream& ostream_, std::string& string_)
//    {
//        uint32_t size = string_.size();
//        write (ostream_, size);
//        write_internal (ostream_, string_.c_str(), string_.size());
//        return ostream_;
//    }

//    static inline ostream& write (ostream& ostream_, const char* str)
//    {
//        uint32_t size = strlen (str);
//        write (ostream_, size);
//        write_internal (ostream_, str, size);
//        return ostream_;
//    }

//    static inline ostream& write (ostream& ostream_, vector<bool>& container)
//    {
//        uint32_t size = container.size();
//        write (ostream_, size);

//        for (auto ite : container)
//        {
//            bool c = ite;
//            write (ostream_, c);
//        }

//        return ostream_;
//    }



//template<typename T>
//static inline void ZeroMem(T& t)
//{
//	memset(&t, 0, sizeof (T));
//}


//template<typename T>
//static inline istream& read(istream& istream_, T&  t_)
//{   if(std::is_fundamental<T>::value)
//    {
//	return read_internal(istream_, (char*)&t_, sizeof (t_));
//    }else
//    {

//      if(std::is_trivial<T>::value)
//           return  read_internal(istream_, (char*)&t_, sizeof(t_));
//        else
//             return  read(istream_, (serialize::I*)&t_);
//    }
//    assert(0);
//    return istream_;
//}

//template<typename T>
//static inline ostream& write(ostream& ostream_, T&  t_)
//{
//    if(std::is_fundamental<T>::value)
//    {
//    return write_internal(ostream_, (const char*)&t_, sizeof(t_));
//    }else
//    {

//        if(std::is_trivial<T>::value)
//           return write_internal(ostream_, (const char*)&t_, sizeof(t_));
//        else
//            return write(ostream_, (serialize::I*)&t_);
//    }


//}



///////////////vector//////////////////////////////////
//template <class T >
//static inline ostream& write(ostream& ostream_, vector<T>& container)
//{
//	uint32_t size = container.size();
//	write(ostream_, size);
//	for (auto& ite : container)
//	{
//		write(ostream_, ite);
//	}
//	return ostream_;
//}

//template <class T >
//static inline  istream& read(istream& istream_, vector<T>&container)
//{
//	if (!istream_.good() || istream_.eof())return istream_;

//	int size;
//	container.clear();
//	read(istream_, size);
//	for (int i = 0; i < size; ++i)
//	{
//		T t;
//		read(istream_, t);
//		container.push_back(  t);
//	}
//	assert(istream_.good());
//	return istream_;
//}

//template <class T >
//static inline ostream&  write(ostream& ostream_, vector<T*>& container)
//{

//	uint32_t size = container.size();

//	write_internal(ostream_, (char*)&size, sizeof (uint32_t));
//	int index = 0;
//	for (auto ite = container.begin(); ite != container.end(); ite++)
//	{

//		if ((*ite) != NULL)
//		{
//			bool  notNULL = true;
//			write(ostream_, notNULL);

//			I* i = dynamic_cast<I*>(*ite);
//			write(ostream_, i);
//		}
//		else
//		{
//			bool  notNULL = false;
//			write(ostream_, notNULL);
//		}
//		index++;
//	}
//	return ostream_;
//}

//template <class T >
//static inline  istream& read(istream& istream_, vector<T*>& container)
//{
//	if (!istream_.good() || istream_.eof())return istream_;
//	int size;
//	container.clear();

//	read(istream_, size);
//	container.resize(size);
//	for (int i = 0; i < size; ++i)
//	{
//		bool  notNULL = false;
//		read(istream_, notNULL);

//		if (notNULL)
//		{
//			T* object = new T;
//			read(istream_, dynamic_cast<I*>(object));
//			container[i] = object;
//		}
//		else
//		{
//			container[i] = NULL;
//		}
//	}
//	return istream_;
//}


//////////////////map/////////////////////

//template <class K, class V>
//static inline ostream&  write(ostream& ostream_, map<K, V>& container)
//{

//	uint32_t size = container.size();
//	write(ostream_, size);

//	for (auto p : container)
//	{
//		write(ostream_, p.first);
//		write(ostream_, p.second);


//	}
//	return ostream_;
//}

//template <class K, class V>
//static inline  istream& read(istream& istream_, map<K, V>& container)
//{
//	if (!istream_.good() || istream_.eof())return istream_;
//	int size;
//	container.clear();
//	read(istream_, size);

//	for (int i = 0; i < size; ++i)
//	{
//		K key;
//		V value;
//		read(istream_, key);

//		read(istream_, value);
//		container[key] = value;

//	}
//	return istream_;
//}


//template <class K, class V>
//static inline ostream&  write(ostream& ostream_, map<K, V*>& container)
//{

//	uint32_t size = container.size();
//	write(ostream_, size);
//	int index = 0;
//	for (auto p : container)
//	{
//		write(ostream_, p.first);

//		if ((p.second) != NULL)
//		{
//			bool notNULL = true;
//			write(ostream_, notNULL);
//			I* i = dynamic_cast<I*>(p.second);
//			write(ostream_, i);
//		}
//		else
//		{
//			bool notNULL = false;
//			write(ostream_, notNULL);
//		}


//		index++;
//	}
//	return ostream_;
//}

//template <class K, class V>
//static inline  istream& read(istream& istream_, map<K, V*>& container)
//{
//	if (!istream_.good() || istream_.eof())return istream_;
//	int size;
//	container.clear();
//	read(istream_, size);

//	for (int i = 0; i < size; ++i)
//	{
//		K key;
//		read(istream_, key);
//		bool  notNULL;
//		read(istream_, notNULL);
//		if (notNULL)
//		{
//			V* object = new V;
//			read(istream_, dynamic_cast<I*>(object));
//			container[key] = (V*)object;
//		}
//		else
//		{
//			container[key] = NULL;
//		}
//	}
//	return istream_;
//}


//////////////////set/////////////////////

//template <class T >
//static inline ostream& write(ostream& ostream_, set<T>& container)
//{
//	uint32_t size = container.size();
//	write(ostream_, size);
//	for (auto& ite : container)
//	{
//		write(ostream_, ite);
//	}
//	return ostream_;
//}

//template <class T >
//static inline  istream& read(istream& istream_, set<T>&container)
//{
//	if (!istream_.good() || istream_.eof())return istream_;

//	int size;
//	container.clear();
//	read(istream_, size);
//	for (int i = 0; i < size; ++i)
//	{
//		T t;
//		read(istream_, t);
//		container.insert(t);
//	}
//	assert(istream_.good());
//	return istream_;
//}

//template <class T >
//static inline ostream&  write(ostream& ostream_, set<T*>& container)
//{

//	uint32_t size = container.size();
//	write(ostream_, size);

//	for (auto ite = container.begin(); ite != container.end(); ite++)
//	{
//		if ((*ite) != NULL)
//		{
//			bool noNULL = true;
//			write(ostream_, noNULL);
//			I* i = dynamic_cast<I*>(*ite);
//			write(ostream_, i);
//		}
//		else
//		{
//			bool noNULL = false;
//			write(ostream_, noNULL);
//		}

//	}
//	return ostream_;
//}

//template <class T >
//static inline  istream& read(istream& istream_, set<T*>& container)
//{
//	if (!istream_.good() || istream_.eof())return istream_;
//	int size;
//	container.clear();

//	read(istream_, size);
//	container.resize(size);
//	for (int i = 0; i < size; ++i)
//	{
//		bool notNULL = false;
//		read(istream_, notNULL);
//		if (notNULL)
//		{
//			T* object = new T;
//			read(istream_, dynamic_cast<I*>(object));
//			container.insert(object);
//		}
//		else
//		{
//			container.insert(NULL);
//		}
//	}
//	return istream_;
//}



//////////////////list/////////////////////

//template <class T >
//static inline ostream& write(ostream& ostream_, list<T>& container)
//{
//	uint32_t size = container.size();
//	write(ostream_, size);
//	for (auto& ite : container)
//	{
//		write(ostream_, ite);
//	}
//	return ostream_;
//}

//template <class T >
//static inline  istream& read(istream& istream_, list<T>&container)
//{
//	if (!istream_.good() || istream_.eof())return istream_;

//	int size;
//	container.clear();
//	read(istream_, size);
//	for (int i = 0; i < size; ++i)
//	{
//		T t;
//		read(istream_, t);
//		container.push_back(t);
//	}
//	assert(istream_.good());
//	return istream_;
//}

//template <class T >
//static inline ostream&  write(ostream& ostream_, list<T*>& container)
//{

//	uint32_t size = container.size();
//	write(ostream_, size);

//	for (auto ite = container.begin(); ite != container.end(); ite++)
//	{
//		if ((*ite) != NULL)
//		{
//			bool noNULL = true;
//			write(ostream_, noNULL);
//			I* i = dynamic_cast<I*>(*ite);
//			write(ostream_, i);
//		}
//		else
//		{
//			bool noNULL = false;
//			write(ostream_, noNULL);
//		}

//	}
//	return ostream_;
//}



//template <class T >
//static inline  istream& read(istream& istream_, list<T*>& container)
//{
//	if (!istream_.good() || istream_.eof())return istream_;
//	int size;
//	container.clear();

//	read(istream_, size);

//	for (int i = 0; i < size; ++i)
//	{
//		bool notNULL = false;
//		read(istream_, notNULL);
//		if (notNULL)
//		{
//			T* object = new T;
//			read(istream_, dynamic_cast<I*>(object));
//			container.push_back(object);
//		}
//		else
//		{
//			container.push_back(NULL);
//		}
//	}
//	return istream_;
//}

//};



















///*******************************************************************************
// * STeCa2 - StressTextureCalculator ver. 2
// *
// * Copyright (C) 2016-7 Forschungszentrum Jülich GmbH
// *
// * This program is free software: you can redistribute it and/or modify it
// * under the terms of the GNU General Public License as published by the Free
// * Software Foundation, either version 3 of the License, or (at your option)
// * any later version.
// *
// * This program is distributed in the hope that it will be useful, but
// * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// * or FITNESS FOR A PARTICULAR PURPOSE.
// *
// * See the COPYING and AUTHORS files for more details.
// ******************************************************************************/

//#include "json.hpp"
#include <dev_lib/inc/defs.inc>
//#include <dev_lib/inc/ptr.hpp>

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

//namespace core_qt {
////------------------------------------------------------------------------------

//static Json toJson(l::ij::rc ij) {
//  return Json()
//    .saveInt(core::key::I, ij.i)
//    .saveInt(core::key::J, ij.j);
//}

//static l::ij toIJ(Json::rc obj) may_err {
//  return l::ij(
//    obj.loadInt(core::key::I),
//    obj.loadInt(core::key::J)
//  );
//}

//TEST_("IJ::json",
//  l::ij ij(-1,2), ij1(toIJ(toJson(ij)));
//  CHECK_EQ(ij, ij1);
//)

//static Json toJson(l::xy::rc xy) {
//  return Json()
//    .saveReal(core::key::X, xy.x)
//    .saveReal(core::key::Y, xy.y);
//}

//static l::xy toXY(Json::rc obj) may_err {
//  return l::xy(
//    obj.loadInt(core::key::X),
//    obj.loadInt(core::key::Y)
//  );
//}

//TEST_("XY::json",
//  l::xy xy(-1,2), xy1(toXY(toJson(xy)));
//  CHECK_EQ(xy, xy1);
//)

////------------------------------------------------------------------------------

//using core::Range;
//using core::Ranges;

//static Json toJson(Range::rc rge) {
//  return Json()
//    .saveReal(core::key::MIN, rge.min)
//    .saveReal(core::key::MAX, rge.max);
//}

//static Range toRange(Json::rc obj) may_err {
//  return Range(
//    obj.loadReal(core::key::MIN),
//    obj.loadReal(core::key::MAX)
//  );
//}

//TEST_("Range::json",
//  Range r(-1,2), r1(toRange(toJson(r)));
//  CHECK_EQ(r, r1);
//)

//static JsonArr toJson(Ranges::rc rs) {
//  JsonArr arr;
//  for_i_(rs.size())
//    arr.append(toJson(rs.at(i)));
//  return arr;
//}

//static Ranges toRanges(JsonArr::rc arr) may_err {
//  Ranges rs;
//  for_i_(arr.count())
//    rs.add(toRange(arr.objAt(i)));
//  return rs;
//}

//TEST_("Ranges::json",
//  Ranges rs;
//  rs.add(Range());
//  rs.add(Range(9));
//  rs.add(Range(-3, -2));
//  rs.add(Range::inf());

//  Ranges rs1(toRanges(toJson(rs)));
//  CHECK_EQ(rs, rs1);
//)

////------------------------------------------------------------------------------
//// TODO impl from fun.cpp

//using core::Par;
//using core::Fun;
//using core::SimpleFun;
//using core::SumFuns;

//static Json toJson(Par::rc par) {
//  return Json()
//    .saveReal(core::key::VALUE, par.val);
//}

//static Par toPar(Json::rc obj) may_err {
//  return Par(obj.loadReal(core::key::VALUE), 0);
//}

//static Json toJson(Fun::rc f);

//static Json toJson(SimpleFun::rc f) {
//  JsonArr arr;
//  for_i_(f.parCount())
//    arr.append(toJson(f.parAt(i)));
//  return Json().saveArr(core::key::PARAMS, arr);
//}

//static Json toJson(SumFuns::rc f) {
//  Json obj;
//  obj.saveStr(core::key::TYPE, core::key::SUM);

//  uint funCount = l::to_uint(f.funs.size());
//  obj.saveUint(core::key::COUNT, funCount);

//  for_i_(funCount)
//    obj.saveObj(CAT(core::key::FUN, (i+1)), toJson(*f.funs.at(i)));

//  return obj;
//}

//static Json toJson(Fun::rc f) {
//  if (dynamic_cast<SimpleFun const*>(&f))
//    return toJson(static_cast<SimpleFun const&>(f));
//  if (dynamic_cast<SumFuns const*>(&f))
//    return toJson(static_cast<SumFuns const&>(f));

//  EXPECT_(false)
//  return Json();
//}

//static l::own<Fun> loadFun(Json::rc obj);

//static void loadSimpleFun(SimpleFun& f, Json::rc obj) {
//  JsonArr arr = obj.loadArr(core::key::PARAMS);
//  for_i_(arr.count())
//    f.add(toPar(arr.objAt(i)));
//}

//static void loadSumFuns(SumFuns& f, Json::rc obj) may_err {
//  EXPECT_(f.funs.isEmpty()) // cannot load twice

//  for_i_(obj.loadUint(core::key::COUNT))
//    f.add(loadFun(obj.loadObj(CAT(core::key::FUN, (i+1)))));
//}

//static l::own<Fun> loadFun(Json::rc obj) may_err {
//  auto f = l::scope(Fun::make(obj.loadStr(core::key::TYPE)));
//  if (dynamic_cast<SimpleFun*>(f.ptr()))
//    loadSimpleFun(static_cast<SimpleFun&>(*f), obj);
//  else if (dynamic_cast<SumFuns*>(f.ptr()))
//    loadSumFuns(static_cast<SumFuns&>(*f), obj);

//  return f.take().justOwn();
//}

//TEST_("Par::json",
//  Par par(6,0), par1(toPar(toJson(par)));
//  CHECK_EQ(par.val,  par1.val);
//  CHECK_EQ(par1.err, 0);
//)

////------------------------------------------------------------------------------

//Json::Json() {}

//Json::Json(QJsonObject const& obj) : base(obj) {}

//#define LOAD_DEF(type)      value(key).isUndefined() ? def : load##type(key)
//#define RET_LOAD_DEF(type)  return LOAD_DEF(type);

//#define key_err(msg) l::err(key + ": " +  msg)

//Json& Json::saveObj(strc key, Json::rc obj) {
//  insert(key, obj);
//  return *this;
//}

//Json Json::loadObj(strc key, bool defEmpty) const may_err {
//  auto val = value(key);

//  switch (val.type()) {
//  case QJsonValue::Object:
//    return val.toObject();
//  case QJsonValue::Undefined:
//    if (defEmpty)
//      return Json();
//    _if_clang_([[clang::fallthrough]];)
//  default:
//    key_err("not an object");
//  }
//}

//Json& Json::saveArr(strc key, JsonArr::rc arr) {
//  insert(key, arr.base_rc());
//  return *this;
//}

//JsonArr Json::loadArr(strc key, bool defEmpty) const may_err {
//  auto val = value(key);

//  switch (val.type()) {
//  case QJsonValue::Array:
//    return val.toArray();
//  case QJsonValue::Undefined:
//    if (defEmpty)
//      return JsonArr();
//    _if_clang_([[clang::fallthrough]];)
//  default:
//    key_err("not an array");
//  }
//}

//Json& Json::saveInt(strc key, int num) {
//  insert(key, num);
//  return *this;
//}

//int Json::loadInt(strc key) const may_err {
//  auto val = value(key);

//  switch (val.type()) {
//  case QJsonValue::Double:
//    return qRound(val.toDouble());
//  default:
//    key_err("bad number format");
//  }
//}

//int Json::loadInt(strc key, int def) const may_err {
//  RET_LOAD_DEF(Int)
//}

//Json& Json::saveUint(strc key, uint num) {
//  return saveInt(key, l::to_i(num));
//}

//uint Json::loadUint(strc key) const may_err {
//  int num = loadInt(key);
//  if (num < 0)
//    key_err("bad number format");
//  return l::to_u(num);
//}

//uint Json::loadUint(strc key, uint def) const may_err {
//  RET_LOAD_DEF(Uint)
//}

//Json& Json::savePint(strc key, pint num) {
//  return saveUint(key, num);
//}

//l::pint Json::loadPint(strc key) const {
//  uint num = loadUint(key);
//  if (num < 1)
//    key_err("expecting a positive integer");
//  return pint(num);
//}

//l::pint Json::loadPint(strc key, uint def) const {
//  return pint(LOAD_DEF(Pint));
//}

//static QString INF_P("+inf"), INF_M("-inf");

//Json& Json::saveReal(strc key, real num) {
//  if (l::isnan(num)) {
//    // do save nothing for nans
//  } else if (l::isinf(num)) {
//    insert(key, num < 0 ? INF_M : INF_P);
//  } else {
//    insert(key, num);
//  }

//  return *this;
//}

//real Json::loadReal(strc key) const may_err {
//  auto val = value(key);

//  switch (val.type()) {
//  case QJsonValue::Undefined:
//    return l::flt_nan;          // not present means nan
//  case QJsonValue::String: {  // infinities stored as strings
//    auto s = val.toString();
//    if (INF_P == s)
//      return +l::flt_inf;
//    if (INF_M == s)
//      return -l::flt_inf;
//    key_err("bad number format");
//  }
//  default:
//    return val.toDouble();
//  }
//}

//real Json::loadReal(strc key, real def) const may_err {
//  RET_LOAD_DEF(Real)
//}

//Json& Json::savePeal(strc key, peal num) {
//  return saveReal(key, num);
//}

//l::peal Json::loadPeal(strc key) const {
//  real num = loadReal(key);
//  if (num <= 0)
//    key_err("expecting a positive real");
//  return peal(num);
//}

//l::peal Json::loadPeal(strc key, peal def) const {
//  RET_LOAD_DEF(Peal)
//}

//Json& Json::saveBool(strc key, bool b) {
//  insert(key, b);
//  return *this;
//}

//bool Json::loadBool(strc key) const may_err {
//  auto val = value(key);

//  switch (val.type()) {
//  case QJsonValue::Bool:
//    return val.toBool();
//  default:
//    key_err("not a boolean");
//  }
//}

//bool Json::loadBool(strc key, bool def) const may_err {
//  RET_LOAD_DEF(Bool)
//}

//Json& Json::saveStr(strc key, strc s) {
//  insert(key, s);
//  return *this;
//}

//str Json::loadStr(strc key) const may_err {
//  auto val = value(key);

//  switch (val.type()) {
//  case QJsonValue::String:
//    return l_qt::fromQt(val.toString());
//  default:
//    key_err("not a string");
//  }
//}

//str Json::loadStr(strc key, strc def) const may_err {
//  RET_LOAD_DEF(Str)
//}

//Json& Json::saveRange(strc key, Range::rc range) {
//  insert(key, toJson(range));
//  return *this;
//}

//Range Json::loadRange(strc key) const may_err {
//  return toRange(loadObj(key));
//}

//Json& Json::saveRanges(strc key, Ranges::rc ranges) {
//  saveArr(key, toJson(ranges));
//  return *this;
//}

//Ranges Json::loadRanges(strc key) const may_err {
//  return toRanges(loadArr(key));
//}

//Json& Json::saveIJ(strc key, l::ij::rc ij) {
//  insert(key, toJson(ij));
//  return *this;
//}

//l::ij Json::loadIJ(strc key) const may_err {
//  return toIJ(loadObj(key));
//}

//Json& Json::saveXY(strc key, l::xy::rc xy) {
//  insert(key, toJson(xy));
//  return *this;
//}

//l::xy Json::loadXY(strc key) const may_err {
//  return toXY(loadObj(key));
//}

//Json& Json::operator+=(Json::rc that) {
//  for (auto& key : that.keys())
//    base::insert(key, that[key]);
//  return *this;
//}

//Json Json::operator+(Json::rc that) const {
//  return Json(*this) += that;
//}

//Json::iterator Json::insert(strc key, strc val) {
//  return base::insert(l_qt::toQt(key), l_qt::toQt(val));
//}

//Json::iterator Json::insert(strc key, QJsonValue const& val) {
//  return base::insert(l_qt::toQt(key), val);
//}

//QJsonValue Json::value(strc key) const {
//  return base::value(l_qt::toQt(key));
//}

////------------------------------------------------------------------------------

//JsonArr::JsonArr() {
//}

//JsonArr::JsonArr(QJsonArray const& array) : base(array) {}

//void JsonArr::append(Json::rc obj) {
//  base::append(obj.base_rc());
//}

//uint JsonArr::count() const {
//  return l::to_u(base::count());
//}

//Json JsonArr::objAt(uint i) const {
//  auto obj = base::at(l::to_i(i));
//  if (QJsonValue::Object != obj.type())
//    l::err(CAT("not an object at: ", i));
//  return base::at(l::to_i(i)).toObject();
//}

////------------------------------------------------------------------------------
//}
//// eof
