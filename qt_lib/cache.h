// (app_lib)

#ifndef APP_LIB_TYP_CACHE_H
#define APP_LIB_TYP_CACHE_H

#include "../inc/defs_h.h"
#include "../inc/num.h"
#include "../inc/ptr.h"
#include <QMap>
#include <QSharedPointer>

namespace l {
//------------------------------------------------------------------------------

dcl_(cache_base)
protected:
  using mru_t = uint32;
  atr_(sz_t,  maxItems)
  mut_(mru_t, nextMru)
  mut_(bool,  rollOver = false)  // L.v.

  cache_base(pint maxItems);
  virtual ~cache_base();

  virtual void trim(sz_t) = 0;

  virtual sz_t size() const = 0;
  void prepAdd() {
    if (rollOver)
      trim(0);
    else if (size() >= maxItems)
      trim(to_sz(qRound(maxItems * .7)));
  }

  mru_t next() {
    auto mru = nextMru++;
    if (0 == nextMru)
      rollOver = true;  // overflow
    return mru;
  }

  void touch(mru_t& mru) {
    if (mru + 1 == nextMru)
      return;
    mru = next();
  }
dcl_end

template <typename Key, typename T>
dcl_reimpl_(cache, cache_base)
private:
  using rcKey = Key const&;
  using Tsh   = QSharedPointer<T>;
  using tsh_mru = std::pair<Tsh, mru_t>;

  using cash_t  = QMap<Key,tsh_mru>;
  using cash_it = typename cash_t::iterator;

  cash_t cash;

public:
  cache(pint n) : base(n) {}

  val_(sz_t, size, (), cash.size())
  val_(bool, isEmpty, (),    cash.isEmpty())
  set_inl_(clear, (),  trim(0); )

  void trim(sz_t n) { // TODO to cache_base (with adapter)
    if (size() > n) {
      QMap<mru_t, cash_it> mit;
      for (auto it = cash.begin(), itEnd = cash.end(); it != itEnd; ++it)
        mit.insert(it->second, it);

      ENSURE_(mit.size() == size()) // no duplicate mrus

      sz_t cnt = size() - n;
      for (auto it = mit.begin(); cnt-- > 0; ++it)
        cash.erase(it.value());
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
