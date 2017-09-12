// (dev_lib)

#pragma once
#include "../defs.hpp"
#include "../inc/num.hpp"
#include <vector>

namespace l {
//------------------------------------------------------------------------------

template <typename T>
dcl_reimpl_(vec, std::vector<T>)
  USING_BASE_(base, begin, end, reserve, resize, erase, clear, at)

#ifdef _WIN32
  mth_(T const*, data, ())    RET_(&(base::operator[](0)))
  mth_mut_(T*, data, ())      RET_(&(base::operator[](0)))
#else
  using base::data;
#endif

  bol_(isEmpty, ())           RET_(base::empty())
  mth_(sz_t,     size, ())    RET_(sz_t(base::size()))
  mth_(T const&, first, ())   RET_(base::front())
  mth_(T const&, last,  ())   RET_(base::back())

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

  mth_mut_(T&, refAt, (uint i))     RET_(base::operator[](i))

  bol_(operator==, (rc that)) RET_(base_rc() == that.base_rc())
  bol_(operator!=, (rc that)) RET_(base_rc() != that.base_rc())
dcl_end

//------------------------------------------------------------------------------
}
// eof
