// (dev_lib)

#pragma once
#include "../defs.hpp"

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
    return own<T>(NEEDED_(ptr()));
  }
};

//------------------------------------------------------------------------------
// expecting an owning over; will take ownership
// only a hint, ownership not enforced

template <typename T>
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
  scoped(own<T> p)      : ptr_base(p) {}
  scoped(scoped&& that) : ptr_base(that.take()) {}
  scoped(scoped const&) = delete;

 ~scoped() {
    drop();
  }

  template <typename O>
  scoped& operator=(scoped<O>& that) SET_(reset(that.take()))
  template <typename O>
  scoped& operator=(own<O>& that)    SET_(reset(that.take()))

  void reset(T* p_) {
    delete static_cast<T*>(mutp(p));
    mut(p) = mut(p_);
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
template <typename T> scoped<T> scope(own<T> p)         RET_(scope(p))

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

// non-mutable
template <typename T>
struct shared : protected _shared_base_ {
  explicit shared(T const* p = nullptr) : _shared_base_(p)    {}
  shared(shared const& that) : _shared_base_(that) {}
 ~shared() { _drop(); }

  T const* ptr()        const RET_(static_cast<T const*>(p()))
  operator T const*()   const RET_(ptr())
  T const* operator->() const RET_(ptr())

  shared& operator=(shared const& that) {
    if (ptr_base::p != that.ptr_base::p) {
      _drop(); mut(ptr_base::p) = that.ptr_base::p; inc();
    }
    RTHIS
  }

  void reset(T* p) {
    *this = shared(p);
  }

  void drop() {
    reset(nullptr);
  }

private:
  void _drop() {
     if (dec()) {
       delete static_cast<T*>(mutp(p()));
       cleanup();
     }
   }
};

// a handy way to make a shared pointer
template <typename T> shared<T> sh(T* p)             RET_(shared<T>(p))
template <typename T> shared<T const> sh(T const* p) RET_(shared<T>(p))
template <typename T> shared<T> sh(own<T> p)         RET_(shared<T>(p.ptr()))

// declare struct as shared
#define SHARED \
  using sh     = l::shared<Self>;

// clone it
#define CLONED \
  virtual mth_(Self*, clone, ()) RET_(new Self(*this))

//------------------------------------------------------------------------------
}

// make a pointed-to value mutable
template <typename T> T* mutp(l::jp<T> const& p) \
  RET_(const_cast<T*>(p.ptr()))

template <typename T> T* mutp(l::shared<T> const& p) \
  RET_(const_cast<T*>(p.ptr()))

//------------------------------------------------------------------------------
// eof
