// (dev_lib)

#pragma once
#include "../defs.hpp"
#include "../inc/num.hpp"
#include "map.hpp"

namespace l {
//------------------------------------------------------------------------------

dcl_base_(cache_base)
  using mru_t = uint32;

  cache_base(pint maxItems);

  virtual mut_(trim, (uint))  = 0;
  virtual mth_(sz_t, size, ())          = 0;

  mut_(prepAdd, ());
  mth_mut_(mru_t, next, ());
  mut_(touch, (mru_t&));

protected:
  atr_(pint,  maxItems);
  mru_t nextMru;
  bool  rollOver = false;  // L.van
dcl_end

template <typename Key, typename T>
dcl_sub_(cache, cache_base)
private:
  using rcKey = Key const&;
  using Tsh   = shp<T>;
  using tsh_mru = std::pair<Tsh, mru_t>;

  using cash_t  = map<Key,tsh_mru>;
  using cash_it = typename cash_t::iterator;

  cash_t cash;

public:
  cache(pint n) : base(n) {}

  mth_(sz_t, size, ())      RET_(cash.size())
  bol_(isEmpty,    ())      RET_(cash.isEmpty())
  mut_(clear, ()) { trim(0); }

  void trim(uint n) { // TODO to cache_base (with adapter)
    if (size() > n) {
      map<mru_t, cash_it> mit;
      for (auto it = cash.begin(), itEnd = cash.end(); it != itEnd; ++it)
        mit.add(it->second.second, it);

      ENSURE_(mit.size() == size()) // no duplicate mrus

      uint cnt = size() - n;
      for (auto it = mit.begin(); cnt-- > 0; ++it)
        cash.erase(it->second);
    }

    if (isEmpty()) { // cleared
      nextMru  = 0;
      rollOver = false;
    }
  }

  Tsh add(rcKey key, Tsh t) {
    EXPECT_(!cash.contains(key))
    prepAdd();
    cash.add(key, tsh_mru(t, next()));
    return t;
  }

  Tsh get(rcKey key) {
    auto it = cash.find(key);
    if (cash.end() == it)
      return Tsh();
    if (rollOver)
      return add(key, take(key));

    auto&& tm = it->second;
    touch(tm.second);
    return tm.first;
  }

  Tsh take(rcKey key) {
    return cash.take(key).first;
  }
dcl_end

//------------------------------------------------------------------------------
}
// eof
