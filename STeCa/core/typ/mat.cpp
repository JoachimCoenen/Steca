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

#include "mat.h"
#include <c2/c/c_cpp>

core_vec3::core_vec3(real _0_, real _1_, real _2_)
: _0(_0_), _1(_1_), _2(_2_) {}

core_mat3::core_mat3(real _00_, real _01_, real _02_,
                     real _10_, real _11_, real _12_,
                     real _20_, real _21_, real _22_)
: _00(_00_), _01(_01_), _02(_02_),
  _10(_10_), _11(_11_), _12(_12_),
  _20(_20_), _21(_21_), _22(_22_) {}

namespace core {
//------------------------------------------------------------------------------

COMPARABLE_IMPL(vec3) {
  RET_COMPARE_IF_NE_THAT(_0)
  RET_COMPARE_IF_NE_THAT(_1)
  RET_COMPARE_IF_NE_THAT(_2)
  return 0;
}

EQ_NE_IMPL(vec3)

//------------------------------------------------------------------------------

COMPARABLE_IMPL(mat3) {
  RET_COMPARE_IF_NE_THAT(_00)
  RET_COMPARE_IF_NE_THAT(_01)
  RET_COMPARE_IF_NE_THAT(_02)
  RET_COMPARE_IF_NE_THAT(_10)
  RET_COMPARE_IF_NE_THAT(_11)
  RET_COMPARE_IF_NE_THAT(_12)
  RET_COMPARE_IF_NE_THAT(_20)
  RET_COMPARE_IF_NE_THAT(_21)
  RET_COMPARE_IF_NE_THAT(_22)
  return 0;
}

EQ_NE_IMPL(mat3)

mat3 mat3::transposed() const {
  return typ(
    _00, _10, _20,
    _01, _11, _21,
    _02, _12, _22
  );
}

mat3 mat3::operator*(rc n) const {
  return typ(
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

mat3 mat3::rotationCWx(c::rad angle) {
  return mat3(1, 0,            0,
              0, angle.cos(), -angle.sin(),
              0, angle.sin(),  angle.cos());
}

mat3 mat3::rotationCWz(c::rad angle) {
  return mat3(angle.cos(), -angle.sin(), 0,
              angle.sin(),  angle.cos(), 0,
              0,            0,           1);
}

mat3 mat3::rotationCCWz(c::rad angle) {
  return rotationCWz(angle).transposed();
}

TEST("vec3",
  vec3 v(1,2,3);

  CHECK_EQ(v._0, 1);
  CHECK_EQ(v._1, 2);
  CHECK_EQ(v._2, 3);
)

TEST("mat3",
  mat3 m1(0,1,2, 3,4,5, 6,7,8), mc(m1);
  mat3 mt(0,3,6, 1,4,7, 2,5,8);

  CHECK_EQ(m1.transposed(), mt);
  CHECK_EQ(mc.transposed().transposed(), m1);

  CHECK_EQ(vec3(17,62,107), m1 * vec3(4,5,6));
  CHECK_EQ(mat3(5,14,23, 14,50,86, 23,86,149), m1*mt);
)

TEST("mat3::rotation",
  real angle = 1;
  auto cwx  = mat3::rotationCWx(angle);
  auto cwz  = mat3::rotationCWz(angle);
  auto ccwz = mat3::rotationCCWz(angle);

  vec3 v(1,2,3);
  CHECK_EQ(v, cwx.transposed()*(cwx*v));
  CHECK_EQ(v, ccwz*(cwz*v));
)

//------------------------------------------------------------------------------
}
// eof
