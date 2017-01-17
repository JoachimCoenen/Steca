// c

#ifndef C_CPP_VEC_HPP
#define C_CPP_VEC_HPP

#include "../c/h_data"
#include <vector>

#define DATA_NS   c
#define DATA_NAME vec

_struct_sub_retempl(std::vector<T>)
  using base::base;
  using base::begin;
  using base::end;
  using base::clear;
  using base::empty;
  using base::size;
  using base::at;

  void push(T&& t) {
    base::push_back(std::move(t));
  }

  void push(T const& t) {
    base::push_back(t);
  }

  void setAt(sz_t i, T&& t) {
     base::operator[](i) = std::move(t);
  }
_struct_sub_end

#undef DATA_NAME
#undef DATA_NS

#endif
// eof
