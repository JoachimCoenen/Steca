// (dev_lib)

#ifndef DEV_LIB_PTR_H
#define DEV_LIB_PTR_H

#include "defs_h.h"
#include <iostream>

typedef void*       pvoid;
typedef void const* pcvoid;

// safer pointer types
// parts adapted from https://github.com/Microsoft/GSL.git

namespace l {
//------------------------------------------------------------------------------

dcl_(ptr_base)
protected:
  ptr_(void, p);

  ptr_base(pcvoid);
  set_(set, (pcvoid));
dcl_end

//------------------------------------------------------------------------------
// just a pointer! (not null)

template <typename T>
struct jp : ptr_base {
  explicit jp (T const* p) : ptr_base(p) {
    EXPECT_(p)
  }

  jp(jp const&)            = default;
  jp& operator=(jp const&) = default;

  // from another type jp
  template <typename O>
  jp (jp<O> const& that) : ptr_base(static_cast<O const*>(that.p)) {}

  template <typename O>
  jp& operator=(jp<O> const& that) SET_(set(static_cast<O*>(that.p)))

  T const* ptr()        const VAL_(static_cast<T const*>(p))
  operator T const*()   const VAL_(ptr())
  T const* operator->() const VAL_(ptr())

  bool operator==(T* const& o) const VAL_(p == o)
  bool operator!=(T* const& o) const VAL_(p != o)

protected:
  void set(T* p_) {
    EXPECT_(p_)
    p = p_;
  }

private:
  // no pointer arithmetics
  jp<T>& operator++()     = delete;
  jp<T>& operator--()     = delete;
  jp<T>  operator++(int)  = delete;
  jp<T>  operator--(int)  = delete;
  jp<T>& operator+ (sz_t) = delete;
  jp<T>& operator+=(sz_t) = delete;
  jp<T>& operator- (sz_t) = delete;
  jp<T>& operator-=(sz_t) = delete;
};

//------------------------------------------------------------------------------
// a non-null pointer owning the pointed-to thing
// only a hint, ownership not enforced

template <typename T>
struct own : jp<T> {
  explicit own(T const* p) : jp<T>(p) {}
  own(own const&)            = default;
  own& operator=(own const&) = default;

  // from another type
  template <typename O>
  own(own<O> const& that) : jp<T>(static_cast<O const*>(that.ptr())) {}

  template <typename O>
  own& operator=(jp<O> const& that) {
    return static_cast<own&>(jp<T>::operator=(that));
  }
};

// helpers
template <typename T> own<T>       owned(T* p)       VAL_(own<T>(p))
template <typename T> own<T const> owned(T const* p) VAL_(own<T>(p))

//------------------------------------------------------------------------------
// a maybe null pointer owning the pointed-to thing
// only a hint, ownership not enforced

template <typename T>
struct own_ptr : ptr_base {
  own_ptr() : ptr_base(nullptr) {}
  own_ptr(T const*const p) : ptr_base(p) {}

  T const* ptr()      const   VAL_(static_cast<T const*>(p))
  T* ptr()                    VAL_(static_cast<T*>(mutp(p)))
  operator T const*() const   VAL_(ptr())
  operator T*()               VAL_(ptr())
  T*       operator->()       VAL_(ptr())
  T const* operator->() const VAL_(ptr())

  set_(set, (T const*const p_)) SET_(mut(p) = p_)

  // convert to non-null
  own<T> justOwn() const {
    return own<T>(NEEDED_(ptr()));
  }
};

//------------------------------------------------------------------------------
// expecting an owning over; will take ownership
// only a hint, ownership not enforced

template <typename T>
struct give_me : own<T> { using base = own<T>;
  using base::base;
  explicit give_me(T*p) : base(p) {}
};

//------------------------------------------------------------------------------
// scoped (auto-destruct)
template <typename T>
struct scoped : ptr_base {
  scoped(T* p = nullptr): ptr_base(p) {}
  scoped(jp<T> p)       : ptr_base(p) {}
  scoped(own_ptr<T> p)  : ptr_base(p) {}
  scoped(own<T> p)      : ptr_base(p) {}
  scoped(scoped&& that) : ptr_base(that.take()) {}
  scoped(scoped const&) = delete;

 ~scoped() {
    reset(nullptr);
  }

  scoped& operator=(scoped<T>&& that) SET_(reset(that.take()))
  scoped& operator=(own<T>&& that)    SET_(reset(that.take()))

  void reset(T* p_) {
    delete static_cast<T*>(mutp(p));
    mut(p) = mut(p_);
  }

  own_ptr<T> take() {
    // T may be const and put const back
    return own_ptr<T>(static_cast<T*>(mutp(take_p(p))));
  }

  T* ptr() const {
    // T may be const and put const back
    return static_cast<T*>(mutp(p));
  }
  operator T*()   const VAL_(ptr())
  T* operator->() const VAL_(ptr())
};

// a handy way to make a scoped pointer
template <typename T> scoped<T> scope(T* p)             VAL_(scoped<T>(p))
template <typename T> scoped<T const> scope(T const* p) VAL_(scoped<T>(p))
template <typename T> scoped<T> scope(own<T> p)         VAL_(scope(p))

//------------------------------------------------------------------------------
// shared - the main way to handle large immutable data
struct _shared_base_ : ptr_base {
protected:
  _shared_base_(pcvoid);
  _shared_base_(_shared_base_ const&);

  pcvoid p() const;

protected:
  void inc();
  bool dec(); // true when reaches zero
  void cleanup();
};

template <typename T>
struct shared : protected _shared_base_ {
  explicit shared(T const* p = nullptr) : _shared_base_(p) {}
  shared(shared const& that) : _shared_base_(that) {}
 ~shared() { _drop(); }

  void drop() {
    *this = shared();
  }

  shared& operator=(shared<T> const& that) {
    if (ptr_base::p != that.ptr_base::p) {
      _drop(); mut(ptr_base::p) = that.ptr_base::p; inc();
    }
    RTHIS
  }

  void reset(T* p) {
    *this = shared(p);
  }

  T const* ptr()        const VAL_(static_cast<T const*>(p()))
  operator T const*()   const VAL_(ptr())
  T const* operator->() const VAL_(ptr())

private:
  void _drop() {
     if (dec()) {
       delete static_cast<T*>(mutp(p()));
       cleanup();
     }
   }
};

// a handy way to make a shared pointer
template <typename T> shared<T> share(T* p)             VAL_(shared<T>(p))
template <typename T> shared<T const> share(T const* p) VAL_(shared<T>(p))
template <typename T> shared<T> share(own<T> p)         VAL_(scope(p))

// declare struct as shared
#define SHARED \
  using sh   = l::shared<Self>; \
  using shrc = sh const&;

//------------------------------------------------------------------------------
}

// make a pointed-to value mutable
template <typename T> T* mutp(l::jp<T> const& p) \
  VAL_(const_cast<T*>(p.ptr()))

template <typename T> T* mutp(l::shared<T> const& p) \
  VAL_(const_cast<T*>(p.ptr()))

//------------------------------------------------------------------------------
#endif
// eof
