// c

#ifndef C_C_PTR_H
#define C_C_PTR_H

#include <c/c/h>

typedef void*       pvoid;
typedef void const* pcvoid;

_c_struct (ptr)
  _ptr(void, p)

  _c_con (ptr, (pcvoid))
  _mth(pcvoid, take, ())
_c_struct_end(ptr)

#if _is_cpp_
namespace c {

// parts adapted from https://github.com/Microsoft/GSL.git

template <typename T>
struct just_ptr : c_ptr { // not null
  static just_ptr from(T const* p) {
    return just_ptr(p);
  }

  just_ptr(just_ptr const&)   = default;
  just_ptr& operator=(just_ptr const&) = default;

  // from another type just_ptr
  template <typename O>
  just_ptr(just_ptr<O> const& that) : c_ptr(static_cast<O const*>(that.p)) {
  }

  template <typename O>
  just_ptr& operator=(just_ptr<O> const& that) {
    set(static_cast<O*>(that.p));
    return *this;
  }

  T const* ptr()        const { return static_cast<T const*>(p); }
  operator T const*()   const { return ptr(); }
  T const* operator->() const { return ptr(); }

  bool operator==(T* const& o) const { return p == o; }
  bool operator!=(T* const& o) const { return p != o; }

protected:
  just_ptr(T const*const p) : c_ptr(p) {
    EXPECT(p)
  }

  void set(T* p_) {
    EXPECT(p_)
    p = p_;
  }

private:
  // no pointer arithmetics
  just_ptr<T>& operator++()       = delete;
  just_ptr<T>& operator--()       = delete;
  just_ptr<T>  operator++(int)    = delete;
  just_ptr<T>  operator--(int)    = delete;
  just_ptr<T>& operator+ (sz_t) = delete;
  just_ptr<T>& operator+=(sz_t) = delete;
  just_ptr<T>& operator- (sz_t) = delete;
  just_ptr<T>& operator-=(sz_t) = delete;
};

template <typename T> // name = a hint
struct own_ptr : c_ptr {
  own_ptr()     : c_ptr(nullptr) {}
  own_ptr(T const*const p) : c_ptr(p)       {}

  T const* ptr()        const { return static_cast<T const*>(p); }
  operator T const*()   const { return ptr(); }
  T const* operator->() const { return ptr(); }

  void set(T const*const p_)  { mut(p) = mut(p_); }
};

template <typename T> // name = a hint
struct own : just_ptr<T> {
  static own from(T* p) {
    return own(p);
  }

  own(own const&)   = default;
  own& operator=(own const&) = default;

  // from another type
  template <typename O>
  own(own<O> const& that) : just_ptr<T>(static_cast<O*>(that.p)) {
  }

  template <typename O>
  own& operator=(just_ptr<O> const& that) {
    return static_cast<own&>(just_ptr<T>::operator=(that));
  }

protected:
  own(T* p) : just_ptr<T>(p) {}
};

template <typename T>
struct scoped : c_ptr {
  scoped(T* p)          : c_ptr(p) {}
  scoped(just_ptr<T> p) : c_ptr(p) {}
  scoped(own_ptr<T> p)  : c_ptr(p) {}
  scoped(own<T> p)      : c_ptr(p) {}
  scoped(scoped&& that) : c_ptr(that.take()) {}
  scoped(scoped const&) = delete;
 ~scoped() { reset(nullptr); }

  void reset(T* p_) {
    delete static_cast<T*>(mut(p));
    mut(p) = p_;
  }

  own_ptr<T> take() {
    return own_ptr<T>(static_cast<T const*>(c_ptr::take()));
  }

  T const* ptr()        const { return static_cast<T const*>(p); }
  operator T const*()   const { return ptr(); }
  T const* operator->() const { return ptr(); }
};

#define scope(p) c::scoped<decltype(p)>(p)

struct _shared_base_ : c_ptr {
protected:
  _shared_base_(pcvoid);
  _shared_base_(_shared_base_ const&);

  _shared_base_& operator=(_shared_base_ const&);

  pcvoid p() const;

  void inc();
  bool dec();
  void cleanup();
};

template <typename T>
struct shared : protected _shared_base_ {
  shared(T const* p = nullptr) : _shared_base_(p) {}
  shared(shared const& that)   : _shared_base_(that) {}
 ~shared() {
    if (dec()) {
      delete static_cast<T*>(mut(p()));
      cleanup();
    }
  }

  shared& operator=(shared const& that) {
    return _shared_base_::operator=(that);
  }

  T const* ptr()        const { return static_cast<T const*>(p()); }
  T*       mut_ptr()    const { return mut(ptr()); }
  operator T const*()   const { return ptr(); }
  T const* operator->() const { return ptr(); }
};

}
#endif

//------------------------------------------------------------------------------
#endif
// eof
