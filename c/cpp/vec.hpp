// (c)

#ifndef C_CPP_VEC_HPP
#define C_CPP_VEC_HPP

#include "../c/c_def.h"
#include <vector>

#define NS___ c
#define DS___ vec

_struct_sub_templ(std::vector<T>)
  using base::base;
  using base::begin;
  using base::end;
  using base::reserve;
  using base::size;
  using base::erase;
  using base::clear;
  using base::empty;
  using base::at;

  void push(T&& t) {
    base::push_back(std::move(t));
  }

  void push(T const& t) {
    base::push_back(t);
  }

  void rem(sz_t i) {
    erase(begin() + i);
  }

  void setAt(sz_t i, T&& t) {
    base::operator[](i) = std::move(t);
  }

  void setAt(sz_t i, T const& t) {
    base::operator[](i) = t;
  }
_struct_sub_end

#undef DS___
#undef NS___

#endif
// eof
