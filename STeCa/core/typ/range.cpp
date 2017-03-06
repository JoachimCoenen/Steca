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

#include "range.hpp"
#include <c2/c/c_cpp>

#include <algorithm>
#undef NAN

core_Range::core_Range(real min_, real max_) : min(min_), max(max_) {}

namespace core {
//------------------------------------------------------------------------------

TEST("Range(min, max)",
  Range r(6,7);
  CHECK_EQ(6, r.min);
  CHECK_EQ(7, r.max);
)

COMPARABLE_IMPL(Range) {
  EXPECT(isDef() && that.isDef())
  RET_COMPARE_IF_NE_THAT(min)
  RET_COMPARE_IF_NE_THAT(max)
  return 0;
}

DEF_EQ_NE_IMPL(Range)

TEST("Range::compare",
  Range rn1, rn2, r(1,2), r1(1,2), r2(2,3);
  CHECK_FALSE((rn1 == rn2));
  CHECK_FALSE((rn1 != rn2));
  CHECK_EQ(r, r1);
  CHECK_NE(r, r2);
)

Range::Range() : Range(c::NAN) {
}

TEST("Range()",
  Range r;
  CHECK(!r.isDef());
  CHECK(c::isnan(r.min));
  CHECK(c::isnan(r.max));
)

Range::Range(real val) : c_base(val, val) {
}

Range::Range(real min, real max) : c_base(min, max) {
  EXPECT(!c::isnan(min) && !c::isnan(max))
  EXPECT(min <= max)
}

TEST("Range(v)",
  Range r(6);
  CHECK_EQ(6, r.min);
  CHECK_EQ(6, r.max);
)

Range::Range(rc that) : Range(that.min, that.max) {}

Range Range::inf() {
  return Range(-c::INF, +c::INF);
}

TEST("Range::inf",
  auto r = Range::inf();
  CHECK(c::isinf(r.min)); CHECK_LE(r.min, 0);
  CHECK(c::isinf(r.max)); CHECK_GE(r.max, 0);
)

bool Range::isDef() const {
  return !(c::isnan(min) || c::isnan(max));
}

TEST("Range::isDef",
  CHECK(!Range().isDef());
  CHECK(Range(0,1).isDef());
)

bool Range::empty() const {
  return !isDef() || min >= max;
}

TEST("Range::empty",
  CHECK(Range().empty());
  CHECK(!Range::inf().empty());
  CHECK(Range(0).empty());
  CHECK(!Range(0,1).empty());
)

real Range::width() const {
  return isDef() ? max - min : c::NAN;
}

TEST("Range::width",
  CHECK(c::isnan(Range().width()));
  CHECK_EQ(0, Range(0).width());
  CHECK(c::isinf(Range(0,c::INF).width()));
  CHECK(c::isinf(Range::inf().width()));
)

real Range::center() const {
  return isDef() ? (min + max) / 2 : c::NAN;
}

TEST("Range::center",
  CHECK(c::isnan(Range().center()));
  CHECK_EQ(0, Range(0).center());
  CHECK(c::isinf(Range(0,c::INF).center()));
  CHECK(c::isnan(Range::inf().center()));
)

Range Range::safeFrom(real v1, real v2) {
  if (!(v1 < v2))
    mutate::swap(v1, v2);
  return Range(v1, v2);
}

TEST("Range::safeFrom",
  CHECK_EQ(Range::safeFrom(2,3), Range(2,3));
  CHECK_EQ(Range::safeFrom(3,2), Range(2,3));
  CHECK_EQ(Range::safeFrom(+c::INF, -c::INF), Range::inf());
)

void Range::extendBy(real val) {
  mut(min) = c::isnan(min) ? val : c::min(min, val);
  mut(max) = c::isnan(max) ? val : c::max(max, val);
}

void Range::extendBy(Range::rc that) {
  extendBy(that.min);
  extendBy(that.max);
}

TEST("Range::extend",
  auto r  = Range(1,2);
  r.extendBy(-1);
  CHECK_EQ(r, Range(-1,2));
  r.extendBy(Range(3,4));
  CHECK_EQ(r, Range(-1,4));
)

bool Range::contains(real val) const {
  return min <= val && val <= max;
}

bool Range::contains(rc that) const {
  return min <= that.min && that.max <= max;
}

TEST("Range::contains",
  auto r = Range(-1, +1);

  CHECK(!Range().contains(r));
  CHECK(!Range().contains(Range()));
  CHECK(!Range().contains(Range::inf()));

  CHECK(Range::inf().contains(r));
  CHECK(Range::inf().contains(Range::inf()));
  CHECK(!Range::inf().contains(Range()));

  CHECK(!r.contains(Range()));
  CHECK(!r.contains(Range::inf()));
  CHECK(!r.contains(c::NAN));
  CHECK(!r.contains(c::INF));

  CHECK(r.contains(r));

  CHECK(r.contains(-1));
  CHECK(r.contains(0));
  CHECK(r.contains(+1));
  CHECK(r.contains(Range(0,1)));
  CHECK(!r.contains(Range(-2,0)));
)

bool Range::intersects(Range::rc that) const {
  return min <= that.max && that.min <= max;
}

TEST("Range::intersects",
  auto r = Range(-1, +1);

  CHECK(!Range().intersects(r));
  CHECK(!Range().intersects(Range()));
  CHECK(!Range().intersects(Range::inf()));

  CHECK(Range::inf().intersects(r));
  CHECK(Range::inf().intersects(Range::inf()));
  CHECK(!Range::inf().intersects(Range()));

  CHECK(!r.intersects(Range()));
  CHECK(r.intersects(Range::inf()));

  CHECK(r.intersects(r));

  CHECK(r.intersects(Range(0,10)));
  CHECK(r.intersects(Range(-2,0)));
)

Range Range::intersect(Range::rc that) const {
  if (!isDef() || !that.isDef())
    return Range();

  auto min_ = c::max(min, that.min), max_ = c::min(max, that.max);
  if (min_ <= max_)
    return Range(min_, max_);

  // disjoint
  auto val = that.min < min ? min : max;
  return Range(val, val); // empty, isDef()
}

TEST("Range::intersect",
  auto r = Range(-1, +1);

  CHECK(!Range().intersect(r).isDef());
  CHECK(!r.intersect(Range()).isDef());
  CHECK(!Range().intersect(Range::inf()).isDef());

  CHECK_EQ(r, Range::inf().intersect(r));
  CHECK_EQ(Range::inf(), Range::inf().intersect(Range::inf()));
  CHECK(!Range::inf().intersect(Range()).isDef());

  CHECK(!r.intersect(Range()).isDef());
  CHECK_EQ(r, r.intersect(Range::inf()));

  CHECK_EQ(r, r.intersect(r));

  CHECK_EQ(Range(0,1), r.intersect(Range(0,10)));
  CHECK_EQ(Range(-1,0), r.intersect(Range(-2,0)));

  auto disjoint = Range(-3,-2);
  CHECK(r.intersect(disjoint).empty());
  CHECK_EQ(r.min, r.intersect(disjoint).min);
)

real Range::bound(real value) const {
  if (isDef() && !c::isnan(value))
    return c::bound(min, value, max);
  return c::NAN;
}

TEST("Range::bound",
  auto r = Range(-1, +1);

  CHECK(c::isnan(Range().bound(0)));
  CHECK(c::isnan(Range().bound(c::INF)));
  CHECK(c::isnan(Range().bound(c::NAN)));
  CHECK_EQ(0, Range::inf().bound(0));
  CHECK(c::isinf(Range::inf().bound(c::INF)));
  CHECK(c::isnan(Range::inf().bound(c::NAN)));

  CHECK_EQ(0,  r.bound(0));
  CHECK_EQ(-1, r.bound(-10));
  CHECK_EQ(-1, r.bound(-c::INF));
  CHECK_EQ(+1, r.bound(+10));
  CHECK_EQ(+1, r.bound(+c::INF));
)

Range::ref Range::operator=(rc that) {
  mut(min) = that.min;
  mut(max) = that.max;
  return *this;
}

//------------------------------------------------------------------------------

bool Ranges::operator==(rc that) const {
  if (empty())
    return that.empty();
  if (that.empty())
    return empty();

  if (size() != that.size())
    return false;

  for_i (size())
    if (at(i) != that.at(i))
      return false;

  return true;
}

TEST("Ranges:=",
  Ranges rs0, rs1, rs2, rs22;
  CHECK(rs1.add(Range(0,1)));
  CHECK(rs2.add(Range(0,1)));  CHECK(rs2.add(Range(3,4)));
  CHECK(rs22.add(Range(3,4))); CHECK(rs22.add(Range(0,1)));
  CHECK_EQ(rs0, Ranges());
  CHECK_NE(rs0, rs1);
  CHECK_NE(rs1, rs0);
  CHECK_NE(rs1, rs2);
  CHECK_EQ(rs2, rs22);
  CHECK_EQ(rs22, rs2);
  CHECK(rs22.add(Range(-c::INF, -1)));

  CHECK_NE(rs22, rs2);
)

bool Ranges::operator!=(rc that) const {
  return !operator==(that);
}

Ranges::Ranges() {}

bool Ranges::add(Range::rc range) {
  if (!range.isDef())
    return false;

  c::vec<Range> newRanges;

  auto addRange = range;
  for (Range::rc r : rs) {
    if (r.contains(range))
      return false;
    if (!range.contains(r)) {
      if (range.intersects(r))
        addRange.extendBy(r);
      else
        newRanges.push(r);
    }
  }

  newRanges.push(addRange);
  rs = newRanges;
  sort();

  return true;
}

bool Ranges::rem(Range::rc remRange) {
  c::vec<Range> newRanges;
  bool changed = false;

  for (Range::rc r : rs) {
    if (!r.intersect(remRange).empty()) {
      changed = true;
      if (r.min < remRange.min)
        newRanges.push(Range(r.min, remRange.min));
      if (r.max > remRange.max)
        newRanges.push(Range(remRange.max, r.max));
    } else {
      newRanges.push(r);
    }
  }

  if (changed)
    rs = newRanges;

  return changed;
}

static bool lessThan(Range::rc r1, Range::rc r2) {
  RET_BOOL_IF_LT_GT(r1.min, r2.min)
  return r1.max < r2.max;
}

void Ranges::sort() {
  std::sort(rs.begin(), rs.end(), lessThan);
}

TEST_CODE(
typedef struct {
  real min, max;
} min_max;

static bool RANGES_EQ(Ranges::rc rs, c::vec<min_max> mm) {
  if (rs.size() != mm.size())
    return false;

  for_i (rs.size()) {
    auto r = rs.at(i);
    auto m = mm.at(i);
    if (r.min != m.min || r.max != m.max)
      return false;
  }

  return true;
}

static bool RANGES_EQ(Ranges::rc rs1, Ranges::rc rs2) {
  if (rs1.size() != rs2.size())
    return false;

  for_i (rs1.size()) {
    if (rs1.at(i) != rs2.at(i))
      return false;
  }

  return true;
}
)

TEST("Ranges",
  Ranges rs;
  CHECK(rs.empty());
  CHECK(RANGES_EQ(rs, rs));
  CHECK(RANGES_EQ(rs, Ranges()));

  Range r1(0,1), r2(1,2), r3(2,3), r4(3,4);

  CHECK(rs.add(r4)); CHECK_FALSE(rs.add(r4));
  CHECK(RANGES_EQ(rs, {{3,4}} ));

  CHECK(rs.add(r1)); CHECK_FALSE(rs.add(r1));
  CHECK(RANGES_EQ(rs, {{0,1}, {3,4}} ));

  CHECK(rs.add(r2));
  CHECK(RANGES_EQ(rs, {{0,2}, {3,4}} ));

  CHECK(rs.add(r3));
  CHECK(RANGES_EQ(rs, {{0,4}} ));

  CHECK(rs.rem(r2)); CHECK_FALSE(rs.rem(r2));
  CHECK(RANGES_EQ(rs, {{0,1}, {2,4}} ));

  rs.clear(); CHECK_FALSE(rs.rem(r1));
)

//------------------------------------------------------------------------------
}
// eof
