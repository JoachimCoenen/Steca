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

#include "range.h"
#include <c/c/cpp>

namespace core {
//------------------------------------------------------------------------------

#ifdef WITH_TESTS
static bool RANGE_EQ(Range::rc r1, Range::rc r2) {
  return r1.min == r2.min && r1.max == r2.max;
}
#endif

TEST("Range::EQ", ({
  Range r1(1,2), r2(r1);
  CHECK(RANGE_EQ(r1, r2));
});)

Range::Range() : Range(c::NAN) {
}

TEST("Range()", ({
  Range r;
  CHECK(c::isnan(r.min));
  CHECK(c::isnan(r.max));
});)

Range::Range(real val) : Range(val, val) {
}

TEST("Range(v)", ({
  Range r(6);
  CHECK_EQ(6, r.min);
  CHECK_EQ(6, r.max);
});)

Range::Range(real min_, real max_) : min(min_), max(max_) {
}

TEST("Range(min, max)", ({
  Range r(6,7);
  CHECK_EQ(6, r.min);
  CHECK_EQ(7, r.max);
});)

Range::Range(rc that) : Range(that.min, that.max) {}

Range Range::infinite() {
  return Range(-c::INF, +c::INF);
}

int Range::compare(rc that) const {
  RET_COMPARE_IF_NE_THAT(min)
  RET_COMPARE_IF_NE_THAT(max)
  return 0;
}

DEF_EQ_NE_IMPL(Range)

TEST("Range::infinite", ({
  auto r = Range::infinite();
  CHECK(c::isinf(r.min)); CHECK_LE(r.min, 0);
  CHECK(c::isinf(r.max)); CHECK_GE(r.max, 0);
});)

void Range::undef() {
  set(c::NAN);
}

bool Range::isDef() const {
  return !c::isnan(min) && !c::isnan(max);
}

TEST("Range::valid", ({
  Range r;
  CHECK(!r.isDef());
  r.set(0, c::NAN);
  CHECK(!r.isDef());
  r.set(0,1);
  CHECK(r.isDef());
  r.undef();
  CHECK(!r.isDef());
  r.set(0);
  CHECK(r.isDef());
});)

bool Range::empty() const {
  return !isDef() || min >= max;
}

TEST("Range::empty", ({
  CHECK(Range().empty());
  CHECK(!Range::infinite().empty());

  Range r(0);
  CHECK(r.empty());
  r.set(0,1);
  CHECK(!r.empty());
  r.set(2, c::NAN);
  CHECK(r.empty());
});)

real Range::width() const {
  return isDef() ? max - min : c::NAN;
}

TEST("Range::width", ({
  CHECK(c::isnan(Range().width()));
  CHECK_EQ(0, Range(0).width());
  CHECK(c::isinf(Range(0,c::INF).width()));
  CHECK(c::isinf(Range::infinite().width()));
});)

real Range::center() const {
  return isDef() ? (min + max) / 2 : c::NAN;
}

TEST("Range::center", ({
  CHECK(c::isnan(Range().center()));
  CHECK_EQ(0, Range(0).center());
  CHECK(c::isnan(Range(0,c::NAN).center()));
  CHECK(c::isinf(Range(0,c::INF).center()));
  CHECK(c::isnan(Range::infinite().center()));
});)

void Range::set(rc that) {
  set(that.min, that.max);
}

void Range::set(real val) {
  set(val, val);
}

void Range::set(real min_, real max_) {
  mut(min) = min_;
  mut(max) = max_;
  EXPECT(!isDef() || min <= max)
}

void Range::safeSet(real v1, real v2) {
  if (v1 > v2)
    unsafe::swap(v1, v2);
  set(v1, v2);
}

Range Range::safeFrom(real v1, real v2) {
  Range range;
  range.safeSet(v1, v2);
  return range;
}

TEST("Range::safe", ({
  auto r = Range::safeFrom(2,3);
  RANGE_EQ(r, Range(2,3));
  r.set(Range::safeFrom(3,2));
  RANGE_EQ(r, Range(2,3));
  r.safeSet(3,4);
  RANGE_EQ(r, Range(3,4));
  r.safeSet(4,3);
  RANGE_EQ(r, Range(3,4));
  r.safeSet(+c::INF, -c::INF);
  RANGE_EQ(r, Range::infinite());
});)

void Range::extendBy(real val) {
  mut(min) = c::isnan(min) ? val : c::min(min, val);
  mut(max) = c::isnan(max) ? val : c::max(max, val);
}

void Range::extendBy(Range::rc that) {
  extendBy(that.min);
  extendBy(that.max);
}

TEST("Range::extend", ({
  auto r  = Range(1,2);
  r.extendBy(-1);
  RANGE_EQ(r, Range(-1,2));
  r.extendBy(Range(3,4));
  RANGE_EQ(r, Range(-1,4));
});)

bool Range::contains(real val) const {
  return min <= val && val <= max;
}

bool Range::contains(Range::rc that) const {
  return min <= that.min && that.max <= max;
}

TEST("Range::contains", ({
  auto r = Range(-1, +1);

  CHECK(!Range().contains(r));
  CHECK(!Range().contains(Range()));
  CHECK(!Range().contains(Range::infinite()));

  CHECK(Range::infinite().contains(r));
  CHECK(Range::infinite().contains(Range::infinite()));
  CHECK(!Range::infinite().contains(Range()));

  CHECK(!r.contains(Range()));
  CHECK(!r.contains(Range::infinite()));
  CHECK(!r.contains(c::NAN));
  CHECK(!r.contains(c::INF));

  CHECK(r.contains(r));

  CHECK(r.contains(-1));
  CHECK(r.contains(0));
  CHECK(r.contains(+1));
  CHECK(r.contains(Range(0,1)));
  CHECK(!r.contains(Range(-2,0)));
});)

bool Range::intersects(Range::rc that) const {
  return min <= that.max && that.min <= max;
}

TEST("Range::intersects", ({
  auto r = Range(-1, +1);

  CHECK(!Range().intersects(r));
  CHECK(!Range().intersects(Range()));
  CHECK(!Range().intersects(Range::infinite()));

  CHECK(Range::infinite().intersects(r));
  CHECK(Range::infinite().intersects(Range::infinite()));
  CHECK(!Range::infinite().intersects(Range()));

  CHECK(!r.intersects(Range()));
  CHECK(r.intersects(Range::infinite()));

  CHECK(r.intersects(r));

  CHECK(r.intersects(Range(0,10)));
  CHECK(r.intersects(Range(-2,0)));
});)

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

TEST("Range::intersect", ({
  auto r = Range(-1, +1);

  CHECK(!Range().intersect(r).isDef());
  CHECK(!r.intersect(Range()).isDef());
  CHECK(!Range().intersect(Range::infinite()).isDef());

  CHECK_EQ(r, Range::infinite().intersect(r));
  CHECK_EQ(Range::infinite(), Range::infinite().intersect(Range::infinite()));
  CHECK(!Range::infinite().intersect(Range()).isDef());

  CHECK(!r.intersect(Range()).isDef());
  CHECK_EQ(r, r.intersect(Range::infinite()));

  CHECK_EQ(r, r.intersect(r));

  CHECK_EQ(Range(0,1), r.intersect(Range(0,10)));
  CHECK_EQ(Range(-1,0), r.intersect(Range(-2,0)));

  auto disjoint = Range(-3,-2);
  CHECK(r.intersect(disjoint).empty());
  CHECK_EQ(r.min, r.intersect(disjoint).min);
});)

real Range::bound(real value) const {
  if (isDef() && !c::isnan(value))
    return c::bound(min, value, max);
  return c::NAN;
}

Range::ref Range::operator=(rc that) {
  mut(min) = that.min;
  mut(max) = that.max;
  return *this;
}

TEST("Range::bound", ({
  auto r = Range(-1, +1);

  CHECK(c::isnan(Range().bound(0)));
  CHECK(c::isnan(Range().bound(c::INF)));
  CHECK(c::isnan(Range().bound(c::NAN)));
  CHECK_EQ(0, Range::infinite().bound(0));
  CHECK(c::isinf(Range::infinite().bound(c::INF)));
  CHECK(c::isnan(Range::infinite().bound(c::NAN)));

  CHECK_EQ(0,  r.bound(0));
  CHECK_EQ(-1, r.bound(-10));
  CHECK_EQ(-1, r.bound(-c::INF));
  CHECK_EQ(+1, r.bound(+10));
  CHECK_EQ(+1, r.bound(+c::INF));
});)

//------------------------------------------------------------------------------

Ranges::Ranges() {}

Ranges::Ranges(rval that) : rs(std::move(that.rs)) {}

bool Ranges::add(Range::rc range) {
  c::vec<Range> newRanges;

  auto addRange = range;
  for (Range::rc r : rs) {
    if (r.contains(range))
      return false;
    if (!range.contains(r)) {
      if (range.intersects(r))
        addRange.extendBy(r);
      else
        newRanges.emplace_back(r);
    }
  }

  newRanges.emplace_back(addRange);
//  c::set_vec(rs, newRanges);
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
        newRanges.emplace_back(Range(r.min, remRange.min));
      if (r.max > remRange.max)
        newRanges.emplace_back(Range(remRange.max, r.max));
    } else {
      newRanges.emplace_back(r);
    }
  }

  if (changed)
    rs.set(newRanges);
  return changed;
}

_sub_struct(r_, Range)
  r_(rc)            = default;
  ref operator=(rc) = default;
_sub_struct_end

static bool lessThan(r_::rc r1, r_::rc r2) {
  RET_BOOL_IF_LT_GT(r1.min, r2.min)
  return r1.max < r2.max;
}

void Ranges::sort() {
//TODO  reinterpret_cast<c::vec<r_>*>(&rs)->sort(lessThan); TODO
}

#ifdef WITH_TESTS

typedef struct {
  real min, max;
} min_max;

static bool RANGES_EQ(Ranges::rc rs, c::vec<min_max> mm) {
  if (rs.count() != mm.size())
    return false;

  for_i (rs.count()) {
    auto r = rs.at(i);
    auto m = mm.at(i);
    if (r.min != m.min || r.max != m.max)
      return false;
  }

  return true;
}

static bool RANGES_EQ(Ranges::rc rs1, Ranges::rc rs2) {
  if (rs1.count() != rs2.count())
    return false;

  for_i (rs1.count()) {
    if (rs1.at(i) != rs2.at(i))
      return false;
  }

  return true;
}

#endif

TEST("Ranges", ({
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
});)

//------------------------------------------------------------------------------
}
// eof
