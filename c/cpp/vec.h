// c

#ifndef C_CPP_VEC_H
#define C_CPP_VEC_H

#include <c/c/h>
#include <vector>
//#include <algorithm>

namespace c {
//------------------------------------------------------------------------------

template<typename T>
_sub_struct(vec, std::vector<T>)
using base::base;
void set(rc);
_sub_struct_end

//_struct(vec) //, std::vector<T const>)

//  vec(sz_t);
//  void setAt(sz_t i, T const&);
//  using base::base;
//  using base::begin;
//  using base::end;
//  using base::resize;
//  using base::clear;
//  using base::at;
//  using base::operator[];

//  bool is_empty() const  { return base::empty();          }
//  sz_t count()    const  { return base::size();           }
//  void push(T&& t)       { base::push_back(std::move(t)); }
//  void push(T const& t)  { base::push_back(t);            }

//_sub_struct_reimpl_end

//template <typename T>
//void set_vec(typename vec<T>::ref dst, typename vec<T>::rc src) {
//  sz_t sz = src.size();
//  dst.resize(sz);
//  for (sz_t i=0; i<sz; ++i)
//    dst[i].set(src.at(i));
//}

//template <typename T>
//void sort_vec(typename vec<T>::ref vec, bool (lt)(T const&,T const&)) {
//  std::sort(vec.begin(), vec.end(), lt);
//}

//------------------------------------------------------------------------------
}
#endif
// eof
