// c

#ifndef C_CPP_MAP_HPP
#define C_CPP_MAP_HPP

#include "../c/h_data"
#include <map>

namespace c {
//------------------------------------------------------------------------------

template <typename K, typename T>
struct map : std::map<K,T> { _typedefs(map) using base = std::map<K,T>;
  using base::base;
  using base::begin;
  using base::end;
  using base::size;
  using base::erase;
  using base::clear;
  using base::empty;
  using base::at;

  T const& add(K const& k, T const& t) {
    return base::operator[](k) = t;
  }

  bool contains(K const& k) const {
    return base::end() != base::find(k);
  }
};

//------------------------------------------------------------------------------
}
#endif
// eof
