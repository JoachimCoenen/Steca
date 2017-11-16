// (lib/dev)

/** @file
 * strong and smart pointer types;
 * ideas in part adapted from https://github.com/Microsoft/GSL.git
 */

#pragma once

#include "../defs.hpp"
#include <type_traits>

typedef void*       pvoid;
typedef void const* pcvoid;

namespace l {

[[noreturn]] void err(pcstr); // forward declaration

//------------------------------------------------------------------------------

/// the base for all smart pointers
dcl_(ptr_base)
protected:
  ptr_(void, p);        ///< the value

  ptr_base(pcvoid);
  set_(_set, (pcvoid));  ///< setter
dcl_end

//------------------------------------------------------------------------------

/// (j)ust (p)ointer - a pointer that cannot be null
template <typename T>
struct jp : ptr_base {
  explicit jp (T const* p) : ptr_base(p) {
    EXPECT_(p)
  }

  jp(jp const&)            = default;
  jp& operator=(jp const&) = default;

  /// conversion constructor - from a subtype
  template <typename O>
  jp (jp<O> const& that) : ptr_base(static_cast<T const*>(that.ptr())) {}

  /// conversion assignment - from a subtype
  template <typename O>
  jp& operator=(jp<O> const&) = delete;

  /// explicit access to raw pointer
  T const* ptr()        const RET_(static_cast<T const*>(p))
  /// conversion to raw pointer
  operator T const*()   const RET_(ptr())
  /// pointer dereference operator
  T const* operator->() const RET_(ptr())

  bool operator==(T* const& o) const RET_(p == o)
  bool operator!=(T* const& o) const RET_(p != o)

private:
  // disallow pointer arithmetics
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

/** @c own is a just pointer that "owns" the pointed-to thing. Unlike the scoped
 * or shared pointers, @c own does not do any work. It is only a hint, and the
 * programmer must take care of things.
 */
template <typename T>
struct own : jp<T> {
  explicit own(T const* p) : jp<T>(p) {}
  own(own const&)            = default;
  own& operator=(own const&) = default;

  /// conversion constructor - from a subtype
  template <typename O>
  own(own<O> const& that) : jp<T>(that.ptr()) {}

  template <typename O>
  own& operator=(jp<O> const&) = delete;
};

/// a helper function to turn a raw pointer into @c own
template <typename T> own<T> owned(T* p) RET_(own<T>(p))

//------------------------------------------------------------------------------

/** @c own_ptr is a nullable @c own
 */
template <typename T>
struct own_ptr : ptr_base {
  own_ptr() : ptr_base(nullptr) {}
  own_ptr(T const* p) : ptr_base(p) {}

  T const* ptr()      const   RET_(static_cast<T const*>(p))
  T* ptr()                    RET_(static_cast<T*>(mutp(p)))
  operator T const*() const   RET_(ptr())
  operator T*()               RET_(ptr())
  T*       operator->()       RET_(ptr())
  T const* operator->() const RET_(ptr())

  /// convert to non-null @c own
  own<T> justOwn() const {
    return own<T>(NEED_(ptr()));
  }
};

//------------------------------------------------------------------------------

/** @c give_me expects to take over an ownership from @c own. It serves as a hint,
 * the programmer must do all the work.
 */
template <typename T>
struct give_me : own<T> { using base = own<T>;
  explicit give_me(T* p) : base(p) {}
  give_me(own<T> p)      : base(p) {}
};

//------------------------------------------------------------------------------

/** @c scoped deletes its pointed-to object, when it goes out of scope */
template <typename T>
struct scoped : ptr_base {
  scoped(T* p = nullptr): ptr_base(p) {}
  scoped(jp<T> p)       : ptr_base(p) {}
  scoped(own_ptr<T> p)  : ptr_base(p) {}
  scoped(scoped&& that) : ptr_base(that.take()) {}
  scoped(scoped const&) = delete;

  scoped& operator=(scoped&& that) {
    reset(that.take());
    return *this;
  }

 ~scoped() {
    drop();
  }

  T* reset(T* p_) {
    delete static_cast<T*>(mutp(p));
    mut(p) = mut(p_);
    return p_;
  }

  void drop() {
    reset(nullptr);
  }

  own_ptr<T> take() {
    auto res = ptr();
    mut(p) = nullptr;
    return res;
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

/// a helper to make @c scoped
template <typename T> scoped<T> scope(T* p)             RET_(scoped<T>(p))
template <typename T> scoped<T const> scope(T const* p) RET_(scoped<T>(p))
template <typename T> scoped<T> scope(own<T> p)         RET_(scoped<T>(p.ptr()))

//------------------------------------------------------------------------------

/** the base of shared pointers
 */
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

/** the templated base of shared pointers
 */
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

/** checks default-constructibility
 */
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

/** makes T()
 */
template <typename T> struct T_maker {
  T_maybe_maker<T, std::is_default_constructible<T>::value> maker;
  T* make() {
    return maker.make();
  }
};

template <typename T> struct shp;

/** reference-counted, (sh)ared pointer-"(r)eference";
 * not nullable, cannot be re-pointed
 */
template <typename T>
struct shr : sh_base<T> {  
  /// default construction; required by Qt metasystem; otherwise also quite useful
  explicit shr() : sh_base<T>(maker.make()) {}
  /// take an ownership
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

/// a maker
template <typename T> T_maker<T> shr<T>::maker;

/** reference-counted, (sh)ared (p)ointer
 * nullable, can be re-pointed
 */
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

/// helpers to make a shared reference-pointer
template <typename T> shr<T> sh(T* p)             RET_(shr<T>(p))
template <typename T> shr<T const> sh(T const* p) RET_(shr<T>(p))
template <typename T> shr<T> sh(own<T> p)         RET_(shr<T>(p.ptr()))

/// declare types for sharing
#define SHARED \
  using shr = l::shr<Self>; \
  using shp = l::shp<Self>;

/// define @c clone
#define CLONED \
  mth_(Self*, clone, ()) RET_(new Self(*this))

//------------------------------------------------------------------------------
}

/// make a pointed-to value mutable
template <typename T> T* mutp(l::jp<T> const& p) \
  RET_(const_cast<T*>(p.ptr()))

/// make a shared referenced value mutable
template <typename T> T* mutp(l::shr<T> const& p) \
  RET_(const_cast<T*>(p.ptr()))

//------------------------------------------------------------------------------
// eof
