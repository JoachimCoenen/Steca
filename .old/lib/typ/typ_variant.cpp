/*******************************************************************************
 * STeCa2 - StressTextureCalculator ver. 2
 *
 * Copyright (C) 2016 Forschungszentrum Jülich GmbH 2016
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

#include "typ_variant.h"

#include "def/def_cmp_impl.h"
#include <QDate>
#include <QMetaType>

#include "test/tests.h"

namespace typ {
//------------------------------------------------------------------------------

bool isNumeric(QVariant const& v) {
  auto type = QMetaType::Type(v.type());

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

#define IMPL_CMP(name, toType)                       \
  int name(QVariant const& v1, QVariant const& v2) { \
    auto val1 = v1.toType(), val2 = v2.toType();     \
    RET_COMPARE_VALUE2(val1, val2)                       \
    return 0;                                        \
  }

IMPL_CMP(cmp_int,  toInt)
IMPL_CMP(cmp_str,  toString)
IMPL_CMP(cmp_real, toDouble)
IMPL_CMP(cmp_date, toDate)

TEST("cmp_int(QVariants)", ({ // not exhaustive, just due diligence
  QVariant v1(1), v2(2);
  CHECK_EQ( 0, cmp_int(v1, v1));
  CHECK_EQ(-1, cmp_int(v1, v2));
  CHECK_EQ(+1, cmp_int(v2, v1));
});)

#undef IMPL_CMP

//------------------------------------------------------------------------------
}
// eof
