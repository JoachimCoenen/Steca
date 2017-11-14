/*******************************************************************************
 * Steca2 - StressTextureCalculator ver. 2
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

#include "mat.hpp"
#include <lib/dev/inc/defs.inc>
#include <float.h>

namespace core {
//------------------------------------------------------------------------------

vec3::vec3(real _0_, real _1_, real _2_)
: _0(_0_), _1(_1_), _2(_2_) {}

//------------------------------------------------------------------------------

mat3::mat3(real _00_, real _01_, real _02_,
           real _10_, real _11_, real _12_,
           real _20_, real _21_, real _22_)
: _00(_00_), _01(_01_), _02(_02_),
  _10(_10_), _11(_11_), _12(_12_),
  _20(_20_), _21(_21_), _22(_22_) {}

mat3 mat3::transposed() const {
  return mat3(
    _00, _10, _20,
    _01, _11, _21,
    _02, _12, _22
  );
}

mat3 mat3::operator*(rc n) const {
  return mat3(
    _00*n._00 + _01*n._10 + _02*n._20,
    _00*n._01 + _01*n._11 + _02*n._21,
    _00*n._02 + _01*n._12 + _02*n._22,

    _10*n._00 + _11*n._10 + _12*n._20,
    _10*n._01 + _11*n._11 + _12*n._21,
    _10*n._02 + _11*n._12 + _12*n._22,

    _20*n._00 + _21*n._10 + _22*n._20,
    _20*n._01 + _21*n._11 + _22*n._21,
    _20*n._02 + _21*n._12 + _22*n._22
  );
}

vec3 mat3::operator*(vec3::rc v) const {
  return vec3(
    _00*v._0 + _01*v._1 + _02*v._2,
    _10*v._0 + _11*v._1 + _12*v._2,
    _20*v._0 + _21*v._1 + _22*v._2
  );
}

mat3 mat3::rotationCWx(l::rad angle) {
  return mat3(1, 0,            0,
              0, angle.cos(), -angle.sin(),
              0, angle.sin(),  angle.cos());
}

mat3 mat3::rotationCWz(l::rad angle) {
  return mat3(angle.cos(), -angle.sin(), 0,
              angle.sin(),  angle.cos(), 0,
              0,            0,           1);
}

mat3 mat3::rotationCCWz(l::rad angle) {
  return rotationCWz(angle).transposed();
}

TEST_CODE_(
  bool almostEq(real r1, real r2) {
    real const eps = .000001;
    return l::abs(r1 - r2) < eps;
  }

  bool almostEq(vec3::rc v1, vec3::rc v2) {
    return
      almostEq(v1._0, v2._0) && almostEq(v1._1, v2._1) && almostEq(v1._2, v2._2);
  }

  bool almostEq(mat3::rc m1, mat3::rc m2) {
    return
      almostEq(m1._00, m2._00) && almostEq(m1._01, m2._01) && almostEq(m1._02, m2._02) &&
      almostEq(m1._10, m2._10) && almostEq(m1._11, m2._11) && almostEq(m1._12, m2._12) &&
      almostEq(m1._20, m2._20) && almostEq(m1._21, m2._21) && almostEq(m1._22, m2._22);
  }
)

TEST_("vec3",
  vec3 v(1,2,3);

  CHECK_EQ(v._0, 1);
  CHECK_EQ(v._1, 2);
  CHECK_EQ(v._2, 3);
)

TEST_("mat3",
  mat3 m1(0,1,2, 3,4,5, 6,7,8), mc(m1);
  mat3 mt(0,3,6, 1,4,7, 2,5,8);

  CHECK(almostEq(m1.transposed(), mt));
  CHECK(almostEq(mc.transposed().transposed(), m1));

  CHECK(almostEq(vec3(17,62,107), m1 * vec3(4,5,6)));
  CHECK(almostEq(mat3(5,14,23, 14,50,86, 23,86,149), m1*mt));
)

TEST_("mat3::rotation",
  l::rad angle(1.);
  auto cwx  = mat3::rotationCWx(angle);
  auto cwz  = mat3::rotationCWz(angle);
  auto ccwz = mat3::rotationCCWz(angle);

  vec3 v(1,2,3);
  CHECK(almostEq(v, cwx.transposed()*(cwx*v)));
  CHECK(almostEq(v, ccwz*(cwz*v)));
)

//------------------------------------------------------------------------------
}
// eof DOCS
