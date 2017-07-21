// (dev_lib)

#pragma once

#include "../defs.hpp"
#include "../inc/num.hpp"
#include <vector>

namespace l {
//------------------------------------------------------------------------------

template <typename T>
dcl_reimpl_(vec, std::vector<T>)
  using base::base;

  using base::begin;
  using base::end;
  using base::cbegin;
  using base::cend;
  using base::back;
  using base::reserve;
  using base::resize;
  using base::erase;
  using base::clear;
  using base::at;
  using base::data;

  bol_(isEmpty, ())           VAL_(base::empty())
  mth_(sz_t, size, ())        VAL_(to_sz(base::size()))
  mth_(T const&, first, ())   VAL_(base::front())
  mth_(T const&, last,  ())   VAL_(base::back())

  set_(operator=, (rc that))  SET_(base::operator=(that))

  set_(add, (T&& t))          SET_(base::push_back(std::move(t)))
  set_(add, (T const& t))     SET_(base::push_back(t))
  set_(rem, (uint i))         SET_(erase(begin() + i))
  set_(rem, ())               SET_(base::pop_back())

  mth_mut_(T const, pop, ()) {
    auto val = last(); rem(); return val;
  }

  set_(setAt, (uint i, T&& t))      SET_(base::operator[](i) = std::move(t))
  set_(setAt, (uint i, T const& t)) SET_(base::operator[](i) = t)

  mth_mut_(T&, refAt, (uint i))     VAL_(base::operator[](i))

  bol_(operator==, (rc that)) VAL_(base_rc() == that.base_rc())
  bol_(operator!=, (rc that)) VAL_(base_rc() != that.base_rc())
dcl_end

//------------------------------------------------------------------------------
}

dcl_sub_(str_vec, l::vec<str>)
  using base::base;
  fry_(str_vec, split, (strc));
dcl_end

//------------------------------------------------------------------------------
// eof
