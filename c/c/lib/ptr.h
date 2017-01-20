// c

#ifndef C_C_PTR_H
#define C_C_PTR_H

#include "../h_data"

typedef void*       pvoid;
typedef void const* pcvoid;

#define DATA_NS   c
#define DATA_NAME ptr

_c_data
  _c_ptr(void, p)
  _c_con((pcvoid))
_c_data_end

#undef DATA_NAME
#undef DATA_NS

#if _is_cpp_
namespace c {
//------------------------------------------------------------------------------

// mutable take & null
template <typename T> T const* take_p(T const*const& p) {
  auto _ = p; mut(p) = nullptr; return _;
}

// parts adapted from https://github.com/Microsoft/GSL.git
template <typename T>
struct just_ptr : c_ptr { // not null
  static just_ptr from(T const* p) {
    return just_ptr(p);
  }

  just_ptr(just_ptr const&)            = default;
  just_ptr& operator=(just_ptr const&) = default;

  // from another type just_ptr
  template <typename O>
  just_ptr(just_ptr<O> const& that) : c_ptr(static_cast<O const*>(that.p)) {}

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
  just_ptr<T>& operator++()     = delete;
  just_ptr<T>& operator--()     = delete;
  just_ptr<T>  operator++(int)  = delete;
  just_ptr<T>  operator--(int)  = delete;
  just_ptr<T>& operator+ (sz_t) = delete;
  just_ptr<T>& operator+=(sz_t) = delete;
  just_ptr<T>& operator- (sz_t) = delete;
  just_ptr<T>& operator-=(sz_t) = delete;
};

// the name 'own' is only a hint, not enforced
template <typename T>
struct own : just_ptr<T> {
  static own from(T* p) {
    return own(p);
  }

  own(own const&)            = default;
  own& operator=(own const&) = default;

  // from another type
  template <typename O>
  own(own<O> const& that) : just_ptr<T>(static_cast<O*>(that.p)) {}

  template <typename O>
  own& operator=(just_ptr<O> const& that) {
    return static_cast<own&>(just_ptr<T>::operator=(that));
  }

protected:
  own(T* p) : just_ptr<T>(p) {}
};

// the name 'own_ptr' is only a hint, not enforced
// may be null
template <typename T>
struct own_ptr : c_ptr {
  own_ptr()    : c_ptr(nullptr) {}
  own_ptr(T const*const p) : c_ptr(p) {}

  T* ptr()        const { return static_cast<T*>(mut(p)); }
  operator T*()   const { return ptr(); }
  T* operator->() const { return ptr(); }

  void set(T const*const p_)  { mut(p) = mut(p_); }

  own<T> justOwn() const {
    EXPECT(p)
    return own<T>::from(*this);
  }
};

// the name 'give_me' is only a hint, not enforced
template <typename T>
struct give_me : own<T> {
  using own<T>::own;
};

// scoped
template <typename T>
struct scoped : c_ptr {
  scoped(T* p = nullptr): c_ptr(p) {}
  scoped(just_ptr<T> p) : c_ptr(p) {}
  scoped(own_ptr<T> p)  : c_ptr(p) {}
  scoped(own<T> p)      : c_ptr(p) {}
  scoped(scoped&& that) : c_ptr(that.take()) {}
  scoped(scoped const&) = delete;

  ~scoped() {
    reset(nullptr);
  }

  scoped& operator=(scoped<T>&& that) {
    reset(that.take());
    return *this;
  }

  scoped& operator=(own<T>&& that) {
    reset(that.take());
    return *this;
  }

  void reset(T* p_) {
    delete static_cast<T*>(mut(p));
    mut(p) = mut(p_);
  }

  own_ptr<T> take() {
    // T may be const and put const back
    return own_ptr<T>(static_cast<T*>(mut(take_p(p))));
  }

  T* ptr() const {
    // T may be const and put const back
    return static_cast<T*>(mut(p));
  }
  operator T*()   const { return ptr(); }
  T* operator->() const { return ptr(); }
};

// a handy way to make a pointer self-destructing
template <typename T> scoped<T> scope(T* p)        { return c::scoped<T>(p); }
template <typename T> scoped<T> scope(c::own<T> p) { return scope(p);        }

// shared - the main way to handle large immutable data
struct _shared_base_ : c_ptr {
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
struct shared final : protected _shared_base_ {
  shared(T const* p = nullptr) : _shared_base_(p) {}
  shared(shared const& that)   : _shared_base_(that) {}
 ~shared() { _drop(); }

  void drop() {
    *this = shared();
  }

  shared& operator=(shared const& that) {
    if (this != &that) {
      _drop(); mut(c_ptr::p) = mut(c_ptr(that).p); inc();
    }
    return *this;
  }

  T const* ptr()        const { return static_cast<T const*>(p()); }
  T*       mut_ptr()    const { return mut(ptr()); }
  operator T const*()   const { return ptr(); }
  T const* operator->() const { return ptr(); }

private:
  void _drop() {
     if (dec()) {
       delete static_cast<T*>(mut(p()));
       cleanup();
     }
   }
};

}
#endif

//------------------------------------------------------------------------------
#endif
// eof
