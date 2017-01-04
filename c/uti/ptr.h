// c

#ifndef C_PTR_H
#define C_PTR_H

#include "../def/num.h"

C_STRUCT(ptr, {
  void* p;
  C_CON(ptr, (void*))
  C_MTH(void* take, ())
})

#if __cpp__
namespace c {

template <typename T>
struct scoped : c_ptr {
  scoped(T* p) : c_ptr(p) {}
  scoped(scoped const&) = delete;
 ~scoped()  { delete static_cast<T*>(p); }

  operator T*() {
    return static_cast<T*>(p);
  }

  T* take() { return static_cast<T*>(c_ptr::take()); }
};

struct counted_base : c_ptr {
  counted_base(void*);
  counted_base(counted_base const&);

  counted_base& operator=(counted_base const&);

  void* p() const;

protected:
  void inc();
  bool dec();
  void cleanup();
};

template <typename T>
struct counted : private counted_base {
  counted(T* p) : counted_base(p) {}
  counted(counted const& that) : counted_base(that) {}
 ~counted() {
    if (!dec()) {
      delete static_cast<T*>(p());
      cleanup();
    }
  }

  counted& operator=(counted const& that) {
    return counted_base::operator=(that);
  }

  operator T*() {
    return static_cast<T*>(p());
  }
};

}
#endif

//------------------------------------------------------------------------------
#endif
// eof
