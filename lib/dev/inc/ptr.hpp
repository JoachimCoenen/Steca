// (dev_lib)

#pragma once
#include "../defs.hpp"
#include <type_traits>

typedef void*       pvoid;
typedef void const* pcvoid;

// safer pointer types
// parts adapted from https://github.com/Microsoft/GSL.git

namespace l {

[[noreturn]] void err(pcstr);

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
  jp (jp<O> const& that) : ptr_base(static_cast<O const*>(that.ptr())) {}

  template <typename O>
  jp& operator=(jp<O> const& that) SET_(set(static_cast<O*>(that.ptr())))

  T const* ptr()        const RET_(static_cast<T const*>(p))
  operator T const*()   const RET_(ptr())
  T const* operator->() const RET_(ptr())

  bool operator==(T* const& o) const RET_(p == o)
  bool operator!=(T* const& o) const RET_(p != o)

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
  jp<T>& operator+ (uint) = delete;
  jp<T>& operator+=(uint) = delete;
  jp<T>& operator- (uint) = delete;
  jp<T>& operator-=(uint) = delete;
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
template <typename T> own<T>       owned(T* p)       RET_(own<T>(p))
template <typename T> own<T const> owned(T const* p) RET_(own<T>(p))

//------------------------------------------------------------------------------
// a maybe null pointer owning the pointed-to thing
// only a hint, ownership not enforced

template <typename T>
struct own_ptr : ptr_base {
  own_ptr() : ptr_base(nullptr) {}
  own_ptr(T const*const p) : ptr_base(p) {}

  T const* ptr()      const   RET_(static_cast<T const*>(p))
  T* ptr()                    RET_(static_cast<T*>(mutp(p)))
  operator T const*() const   RET_(ptr())
  operator T*()               RET_(ptr())
  T*       operator->()       RET_(ptr())
  T const* operator->() const RET_(ptr())

  set_(set, (T const*const p_)) SET_(mut(p) = p_)

  // convert to non-null
  own<T> justOwn() const {
    return own<T>(NEED_(ptr()));
  }
};

//------------------------------------------------------------------------------
// expecting an owning over; will take ownership
// only a hint, ownership not enforced

template <typename T> // TODO implicit T const ?
struct give_me : own<T> { using base = own<T>;
  explicit give_me(T* p) : base(p) {}
  give_me(own<T> p)      : base(p) {}
};

//------------------------------------------------------------------------------
// scoped (auto-destruct)
template <typename T>
struct scoped : ptr_base {
  scoped(T* p = nullptr): ptr_base(p) {}
  scoped(jp<T> p)       : ptr_base(p) {}
  scoped(own_ptr<T> p)  : ptr_base(p) {}
  scoped(scoped&& that) : ptr_base(that.take()) {}
  scoped(scoped const&) = delete;

 ~scoped() {
    drop();
  }

  template <typename O>
  scoped& operator=(scoped<O>& that) SET_(reset(that.take()))
  template <typename O>
  scoped& operator=(own<O>& that)    SET_(reset(that.take()))

  T* reset(T* p_) {
    delete static_cast<T*>(mutp(p));
    mut(p) = mut(p_);
    return p_;
  }

  void drop() {
    reset(nullptr);
  }

  own_ptr<T> take() {
    return own_ptr<T>(static_cast<T*>(mutp(take_p(p))));
  }

  own<T> takeOwn() {
    return take().justOwn();
  }

  T* ptr() const {
    return static_cast<T*>(mutp(p));
  }
  operator T*()   const RET_(ptr())
  T* operator->() const RET_(ptr())
};

// a handy way to make a scoped pointer
template <typename T> scoped<T> scope(T* p)             RET_(scoped<T>(p))
template <typename T> scoped<T const> scope(T const* p) RET_(scoped<T>(p))
template <typename T> scoped<T> scope(own<T> p)         RET_(scoped<T>(p.ptr()))

//------------------------------------------------------------------------------
// shared - for reference-counted data

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
struct sh_base : protected _shared_base_ {
 ~sh_base() { _drop(); }

  T const* ptr()      const RET_(static_cast<T const*>(p()))
  operator T const*() const RET_(ptr())

protected:
  using _shared_base_::_shared_base_;

  void _set(sh_base const& that) {
    if (ptr_base::p != that.ptr_base::p) {
      _drop(); mut(ptr_base::p) = that.ptr_base::p; inc();
    }
  }

  void _drop() {
     if (dec()) {
       delete static_cast<T*>(mutp(p()));
       cleanup();
     }
   }
};

template <typename T> struct shp;

template <typename T, bool constructible> struct T_maybe_maker;

template <typename T> struct T_maybe_maker<T, false> {
  T* make() {
    static_assert(std::is_abstract<T>::value || std::is_default_constructible<T>::value, "cannot make T");
    err("cannot make T");
  }
};

template <typename T> struct T_maybe_maker<T, true> {
  T* make() {
    static_assert(std::is_default_constructible<T>::value, "");
    return new T;
  }
};

template <typename T> struct T_maker {
  T_maybe_maker<T, std::is_default_constructible<T>::value> maker;
  T* make() {
    return maker.make();
  }
};

// non-mutable shared pointer and reference
template <typename T>
struct shr : sh_base<T> {
  // default () forced by Qt metasystem, but quite useful, too
  explicit shr() : sh_base<T>(maker.make()) {}
  explicit shr(T const* p) : sh_base<T>(p)  {}

  shr(shr const& that) : sh_base<T>(that) {}
  T const& operator()() const {
    EXPECT_(sh_base<T>::ptr())
    return *sh_base<T>::ptr();
  }

  void construct() {
    *this = shr(maker.make());
  }

  void clone() {
    *this = shr(this().clone());
  }

  shr& operator=(shr const& that) {
    sh_base<T>::_set(that);
    RTHIS
  }

  bool operator==(shr<T> const& that) {
    return this->ptr() == that.ptr();
  }

  bool operator==(shp<T> const& that) {
    return this->ptr() == that.ptr();
  }

  bool operator!=(shr<T> const& that) {
    return this->ptr() != that.ptr();
  }

  bool operator!=(shp<T> const& that) {
    return this->ptr() != that.ptr();
  }

private:
  operator bool() const;  // not to be used
  static T_maker<T> maker;
};

template <typename T> T_maker<T> shr<T>::maker;

template <typename T>
struct shp : sh_base<T> {
  explicit shp(T const* p = nullptr) : sh_base<T>(p) {}

  shp(shp const& that)    : sh_base<T>(that) {}
  shp(shr<T> const& that) : sh_base<T>(that) {}

  T const* operator->() const {
    EXPECT_(sh_base<T>::ptr())
    return sh_base<T>::ptr();
  }

  T const& operator*() const {
    EXPECT_(sh_base<T>::ptr())
    return *sh_base<T>::ptr();
  }

  shp& operator=(shp const& that) {
    sh_base<T>::_set(that);
    RTHIS
  }

  bool operator==(shp<T> const& that) {
    return this->ptr() == that.ptr();
  }

  bool operator==(shr<T> const& that) {
    return this->ptr() == that.ptr();
  }

  bool operator!=(shp<T> const& that) {
    return this->ptr() != that.ptr();
  }

  bool operator!=(shr<T> const& that) {
    return this->ptr() != that.ptr();
  }

  shr<T> sh() const {
    return shr<T>(*reinterpret_cast<l::shr<T> const*>(this));
  }

  void reset(T const* p) {
    *this = shp(p);
  }

  void drop() {
    *this = shp(nullptr);
  }
};

// a handy way to make a shr
template <typename T> shr<T> sh(T* p)             RET_(shr<T>(p))
template <typename T> shr<T const> sh(T const* p) RET_(shr<T>(p))
template <typename T> shr<T> sh(own<T> p)         RET_(shr<T>(p.ptr()))

// declare struct as shr
#define SHARED \
  using shr = l::shr<Self>; \
  using shp = l::shp<Self>;

// clone it
#define CLONED \
  virtual mth_(Self*, clone, ()) RET_(new Self(*this))

//------------------------------------------------------------------------------
}

// make a pointed-to value mutable
template <typename T> T* mutp(l::jp<T> const& p) \
  RET_(const_cast<T*>(p.ptr()))

template <typename T> T* mutp(l::shr<T> const& p) \
  RET_(const_cast<T*>(p.ptr()))

//------------------------------------------------------------------------------
// eof
