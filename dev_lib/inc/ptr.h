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
  ptr_(void, p);

  ptr_base(pcvoid);
//TODO  ptr_base& operator=(ptr_base const&);
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
  jp& operator=(jp<O> const& that) {
    set(static_cast<O*>(that.p)); RT
  }

  T const* ptr()        const { return static_cast<T const*>(p);       }
//TODO  T* ptr()                    { return mutp(static_cast<T const*>(p)); }
  operator T const*()   const { return ptr(); }
  T const* operator->() const { return ptr(); }

  bool operator==(T* const& o) const { return p == o; }
  bool operator!=(T* const& o) const { return p != o; }

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
  explicit own(T* p) : jp<T>(p) {}
  own(own const&)            = default;
  own& operator=(own const&) = default;

  // from another type
  template <typename O>
  own(own<O> const& that) : jp<T>(static_cast<O const*>(that.p)) {}

  template <typename O>
  own& operator=(jp<O> const& that) {
    return static_cast<own&>(jp<T>::operator=(that));
  }
};

// helpers
template <typename T> own<T>       owned(T* p)       { return own<T>(p); }
template <typename T> own<T const> owned(T const* p) { return own<T>(p); }

//------------------------------------------------------------------------------
// a maybe null pointer owning the pointed-to thing
// only a hint, ownership not enforced

template <typename T>
struct own_ptr : ptr_base {
  own_ptr() : ptr_base(nullptr) {}
  own_ptr(T const*const p) : ptr_base(p) {}

  T const* ptr()      const   { return static_cast<T const*>(p); }
  T* ptr()                    { return static_cast<T*>(mutp(p)); }
  operator T const*() const   { return ptr(); }
  operator T*()               { return ptr(); }
  T*       operator->()       { return ptr(); }
  T const* operator->() const { return ptr(); }

  set_(set, (T const*const p_))  { mut(p) = p_; RT }

  // convert to non-null
  own<T> justOwn() const {
    EXPECT_(p)
    return own<T>(*this);
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

  scoped& operator=(scoped<T>&& that) {
    reset(that.take()); RT
  }

  scoped& operator=(own<T>&& that) {
    reset(that.take()); RT
  }

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
  operator T*()   const { return ptr(); }
  T* operator->() const { return ptr(); }
};

// a handy way to make a scoped pointer
template <typename T> scoped<T> scope(T* p)             { return scoped<T>(p); }
template <typename T> scoped<T const> scope(T const* p) { return scoped<T>(p); }
template <typename T> scoped<T> scope(own<T> p)         { return scope(p);     }

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
  shared(shared const& that)   : _shared_base_(that) {}
 ~shared() { _drop(); }

  void drop() {
    *this = shared();
  }

  shared& operator=(shared const& that) {
    if (this != &that) {
      _drop(); mut(ptr_base::p) = mut(c_ptr(that).p); inc();
    }
    RT
  }

  void reset(T* p) {
    *this = shared(p);
  }

  T const* ptr()        const { return static_cast<T const*>(p()); }
  operator T const*()   const { return ptr(); }
  T const* operator->() const { return ptr(); }

private:
  void _drop() {
     if (dec()) {
       delete static_cast<T*>(mutp(p()));
       cleanup();
     }
   }
};

// a handy way to make a shared pointer
template <typename T> shared<T> share(T* p)             { return shared<T>(p); }
template <typename T> shared<T const> share(T const* p) { return shared<T>(p); }
template <typename T> shared<T> share(own<T> p)         { return scope(p);     }

//------------------------------------------------------------------------------
}

// make a pointed-to value mutable
template <typename T> T* mutp(l::jp<T> const& p) \
  { return const_cast<T*>(p.ptr()); }

//------------------------------------------------------------------------------
#endif
// eof
