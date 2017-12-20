// ************************************************************************** //
//
//  Steca2: stress and texture calculator
//
//! @file      core/typ/fun.cpp
//! @brief     Implements classes Function, SimpleFunction, SumFunctions
//!
//! @homepage  https://github.com/scgmlz/Steca2
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2017
//! @authors   Scientific Computing Group at MLZ (see CITATION, MAINTAINER)
//
// ************************************************************************** //

#include "def/idiomatic_for.h"
#include "def/special_pointers.h"
#include "typ/exception.h"
#include "typ/fun.h"
#include "typ/str.h"
#include <QJsonArray>

namespace json_fun_key {
str const SUM("sum");
}

namespace typ {

// ************************************************************************** //
//   class Function
// ************************************************************************** //

not_null<Function*> Function::Factory::make(JsonObj const& obj) THROWS {
    str funType = obj.loadString("type");
    Function* fun = super::make(funType);
    RUNTIME_CHECK(fun, "factory does not know " % funType);
    scoped<Function*> f(fun);
    fun->from_json(obj); // may throw
    return not_null<Function*>::from(f.take());
}

Function::Factory Function::factory_;

void Function::addFactoryMaker(rcstr key, not_null<Factory::MakerBase*> maker) {
    factory_.addMaker(key, maker);
}

void Function::initFactory() {
    ONLY_ONCE
    addFactoryMaker(
        json_fun_key::SUM,
        not_null<Factory::MakerBase*>::from(new Factory::Maker<SumFunctions>));
}

not_null<Function*> Function::make(JsonObj const& obj) {
    return factory_.make(obj);
}

Function::Parameter::Parameter() : value_(0), error_(0), range_(Range::infinite()) {}

Range Function::Parameter::valueRange() const {
    return range_.isValid() ? range_ : Range(value_, value_);
}

void Function::Parameter::setValueRange(qreal min, qreal max) {
    range_.set(min, max);
}

void Function::Parameter::setValue(qreal value, qreal error) {
    value_ = value;
    error_ = error;
}

JsonObj Function::Parameter::to_json() const {
    JsonObj ret;
    ret.saveQreal("value", value_);
    ret.insert("range", range_.to_json());
    return ret;
}

void Function::Parameter::from_json(JsonObj const& obj) THROWS {
    value_ = obj.loadQreal("value");
    range_ = obj.loadRange("range");
}

JsonObj Function::to_json() const {
    // nothing to do
    return JsonObj();
}

void Function::from_json(JsonObj const&) THROWS {
    // nothing to do
}


// ************************************************************************** //
//   class SimpleFunction
// ************************************************************************** //

void SimpleFunction::setParameterCount(uint count) {
    parameters_.fill(Parameter(), count);
}

uint SimpleFunction::parameterCount() const {
    return parameters_.count();
}

Function::Parameter& SimpleFunction::parameterAt(uint i) {
    return parameters_[i];
}

void SimpleFunction::reset() {
    for_i (parameters_.count()) {
        auto& p = parameters_[i];
        p.setValue(p.valueRange().bound(0), 0);
    }
}

JsonObj SimpleFunction::to_json() const {
    QJsonArray params;
    for (const Parameter& param : parameters_)
        params.append(param.to_json().sup());
    return super::to_json() + JsonObj().saveArr("parameters", params);
}

void SimpleFunction::from_json(JsonObj const& obj) THROWS {
    super::from_json(obj);
    QJsonArray params = obj.loadArr("parameters");
    uint parCount = params.count();
    setParameterCount(parCount);
    for_i (parCount)
        parameters_[i].from_json(params.at(i).toObject());
}

qreal SimpleFunction::parValue(uint i, qreal const* parValues) const {
    return parValues ? parValues[i] : parameters_.at(i).value();
}

void SimpleFunction::setValue(uint i, qreal val) {
    parameters_[i].setValue(val, 0);
}


// ************************************************************************** //
//   class SumFunction
// ************************************************************************** //

SumFunctions::~SumFunctions() {
    for (Function* f : functions_)
        delete f;
}

void SumFunctions::addFunction(not_null<Function*> function) {
    uint parIndex = parameterCount();
    functions_.append(function);
    for_i (function->parameterCount()) {
        // aggregate parameter list
        allParameters_.append(&function->parameterAt(i));
        // lookup helpers
        function4parIndex_.append(function);
        firstParIndex4parIndex_.append(parIndex);
    }
}

uint SumFunctions::parameterCount() const {
    return allParameters_.count();
}

Function::Parameter& SumFunctions::parameterAt(uint i) {
    return *allParameters_.at(i);
}

qreal SumFunctions::y(qreal x, qreal const* parValues) const {
    qreal sum = 0;
    for (Function* f : functions_) {
        sum += f->y(x, parValues);
        if (parValues)
            parValues += f->parameterCount(); // advance to next function
    }
    return sum;
}

qreal SumFunctions::dy(qreal x, uint parIndex, qreal const* parValues) const {
    Function* f = function4parIndex_.at(parIndex); // aggregate parIndex refers to function f
    // offset parameter index
    uint firstIndex = firstParIndex4parIndex_.at(parIndex);
    if (parValues)
        parValues += firstIndex;
    debug::ensure(firstIndex <= parIndex);
    parIndex -= firstIndex;
    debug::ensure(parIndex < f->parameterCount());
    return f->dy(x, parIndex, parValues);
}

JsonObj SumFunctions::to_json() const {
    JsonObj obj;
    obj.saveString("type", json_fun_key::SUM);
    uint funCount = functions_.count();
    obj.saveUint("count", funCount);
    for_i (funCount)
        obj.saveObj(QString("f%1").arg(i + 1), functions_.at(i)->to_json());
    return Function::to_json() + obj;
}

void SumFunctions::from_json(JsonObj const& obj) THROWS {
    RUNTIME_CHECK(functions_.isEmpty(), "non-empty sum of functions; cannot load twice");
    Function::from_json(obj);
    uint funCount = obj.loadUint("count");
    for_i (funCount) {
        auto funObj = obj.loadObj(QString("f%1").arg(i + 1));
        addFunction(make(funObj));
    }
}

} // namespace typ
