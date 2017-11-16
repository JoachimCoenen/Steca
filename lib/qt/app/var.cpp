// (lib/qt)

#include "var.hpp"
#include "../defs.inc"
#include <QDateTime>

namespace l_qt {
//------------------------------------------------------------------------------

var::var() : Self(QVariant()) {}

var::var(int   v)  : base(v)          {}
var::var(uint  v)  : base(v)          {}
var::var(pcstr v)  : Self(QString(v)) {}
var::var(strc  v)  : Self(toQt(v))    {}
var::var(flt32  v) : base(v)          {}
var::var(flt64  v) : base(v)          {}

var::var(QVariant const& v) {
  *static_cast<QVariant*>(this) = v;
}

bool var::isNum() const {
  auto type = QMetaType::Type(QVariant::type());

  switch (type) {
  case QMetaType::Int:
  case QMetaType::UInt:
  case QMetaType::LongLong:
  case QMetaType::ULongLong:
  case QMetaType::Double:
  case QMetaType::Long:
  case QMetaType::Short:
  case QMetaType::ULong:
  case QMetaType::UShort:
  case QMetaType::Float:
    return true;
  default:
    return false;
  }
}

bool var::isReal() const {
  auto type = QMetaType::Type(QVariant::type());

  switch (type) {
  case QMetaType::Double:
  case QMetaType::Float:
    return true;
  default:
    return false;
  }
}

int var::toInt() const may_err {
  bool ok;
  auto val = base::toInt(&ok);
  check_or_err_(ok, "bad int var");
  return val;
}

uint var::toUInt() const may_err {
  bool ok;
  auto val = base::toUInt(&ok);
  check_or_err_(ok, "bad uint var");
  return val;
}

str var::toStr() const {
  return fromQt(base::toString());
}

real var::toReal() const may_err {
  bool ok;
  auto val = base::toDouble(&ok);
  check_or_err_(ok, "bad int var");
  return val;
}

tim var::toTim() const {
  return tim(msec_t(base::toDateTime().toMSecsSinceEpoch()));
}

int var::cmp_int(var::rc v1, var::rc v2) {
  auto val1 = v1.toInt(), val2 = v2.toInt();
  RET_COMPARE_IF_NE(val1, val2)
  return 0;
}

int var::cmp_str(var::rc v1, var::rc v2) {
  auto val1 = v1.toStr(), val2 = v2.toStr();
  RET_COMPARE_IF_NE(val1, val2)
  return 0;
}

int var::cmp_real(var::rc v1, var::rc v2) {
  auto val1 = v1.toReal(), val2 = v2.toReal();
  RET_COMPARE_IF_NE(val1, val2)
  return 0;
}

int var::cmp_tim(var::rc v1, var::rc v2) {
  auto val1 = v1.toTim().msec, val2 = v2.toTim().msec;
  RET_COMPARE_IF_NE(val1, val2)
  return 0;
}

//------------------------------------------------------------------------------
}
// eof
