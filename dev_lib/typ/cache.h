// (dev_lib)

#ifndef DEV_LIB_CACHE_H
#define DEV_LIB_CACHE_H

#include "../inc/defs_h.h"
#include "../inc/num.h"
#include "../inc/ptr.h"
#include "map.h"

namespace l {
//------------------------------------------------------------------------------

dcl_base_(cache_base)
  using mru_t = uint32;

  atr_(sz_t,  maxItems);
  mut_(mru_t, nextMru);
  mut_(bool,  rollOver = false);  // L.van

  cache_base(pint maxItems);

  virtual mth_mut_(void, trim, (sz_t))  = 0;
  virtual mth_(sz_t, size, ())          = 0;

  mth_mut_(void, prepAdd, ());
  mth_mut_(mru_t, next, ());
  mth_mut_(void, touch, (mru_t&));
dcl_end

template <typename Key, typename T>
dcl_reimpl_(cache, cache_base)
private:
  using rcKey = Key const&;
  using Tsh   = shared<T>;
  using tsh_mru = std::pair<Tsh, mru_t>;

  using cash_t  = map<Key,tsh_mru>;
  using cash_it = typename cash_t::iterator;

  cash_t cash;

public:
  cache(pint n) : base(n) {}

  mth_(sz_t, size, ())      { return cash.size();     }
  bol_(isEmpty,    ())      { return cash.isEmpty();  }
  mth_mut_(void, clear, ()) { trim(0);                }

  void trim(sz_t n) { // TODO to cache_base (with adapter)
    if (size() > n) {
      map<mru_t, cash_it> mit;
      for (auto it = cash.begin(), itEnd = cash.end(); it != itEnd; ++it)
        mit.add(it->second.second, it);

      ENSURE_(mit.size() == size()) // no duplicate mrus

      sz_t cnt = size() - n;
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

    auto &tm = it->second;
    touch(tm.second);
    return tm.first;
  }

  Tsh take(rcKey key) {
    return cash.take(key).first;
  }
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
