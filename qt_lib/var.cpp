// (app_lib)

#include "var.h"
#include "../inc/defs_cpp.h"
#include <QDateTime>

namespace l_qt {
//------------------------------------------------------------------------------

var::var() : Self(QVariant()) {}

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

int var::toInt() const {
  return base::toInt();
}

str var::toStr() const {
  return base::toString();
}

real var::toReal() const {
  return base::toDouble();
}

tim var::toTim() const {
  return tim(msec_t(base::toDateTime().toMSecsSinceEpoch()));
}

int cmp_int(var::rc v1, var::rc v2) {
  auto val1 = v1.toInt(), val2 = v2.toInt();
  RET_COMPARE_IF_NE(val1, val2)
  return 0;
}

int cmp_str(var::rc v1, var::rc v2) {
  auto val1 = v1.toStr(), val2 = v2.toStr();
  RET_COMPARE_IF_NE(val1, val2)
  return 0;
}

int cmp_real(var::rc v1, var::rc v2) {
  auto val1 = v1.toReal(), val2 = v2.toReal();
  RET_COMPARE_IF_NE(val1, val2)
  return 0;
}

int cmp_tim(var::rc v1, var::rc v2) {
  auto val1 = v1.toTim().msec, val2 = v2.toTim().msec;
  RET_COMPARE_IF_NE(val1, val2)
  return 0;
}

//------------------------------------------------------------------------------
}
// eof
