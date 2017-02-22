// (c)
// data defining macros

#ifndef C_C_DEF_H
#define C_C_DEF_H

#include "c.h"

#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wreserved-id-macro"
#endif

//------------------------------------------------------------------------------
// names

#define CAT_(a, b, c) a ## b ## c
#define CAT3(a, b, c) CAT_(a, b, c)

#define NS_NAME_C   CAT3(NS___, _, DS___)
#define NS_NAME_CPP CAT3(NS___, :, DS___)

//------------------------------------------------------------------------------
// declarations

// "C" ns-prefixed struct with the very default constructor ...
#define _c_struct     \
  EXTERN_C struct NS_NAME_C {
#define _c_struct_end \
  }; EXTERN_C_END

// ... that has only immutable data ...
#define _atr(typ, name)     \
  typ const name;
#define _ptr(typ, name)     \
  typ const * const name;

// ... and perhaps constructors ...
#if _is_cpp_
#define _c_con(...) NS_NAME_C(__VA_ARGS__);
#else
#define _c_con(...)
#endif

// ... in cpp ...
#if _is_cpp_

// ... with handy typedefs ...
#define _typedefs(s)      \
  typedef s typ;          \
  typedef typ const& rc;  \
  typedef typ& ref;       \
  typedef typ&& rval;

#define _cpp_struct_typedefs _typedefs(DS___)

// ... extended ...
#define _cpp_struct     \
  namespace NS___ {   \
  struct DS___ : NS_NAME_C { _cpp_struct_typedefs using c_base = NS_NAME_C;
#define _cpp_struct_end \
  };}

// ... by constructors and destructor ...
#define _con(...)               \
  DS___(__VA_ARGS__);
#define _xon(...)               \
  explicit _con(__VA_ARGS__)
#define _con_fwd(args, fwd)     \
  DS___ args : base fwd {}
#define _con_c_fwd(args, fwd)   \
  DS___ args : c_base fwd {}
#define _des()                  \
 ~DS___();

// ... methods ...
#define _mth(typ, mth, args)              \
  typ mth args const;
#define _mth_inline(typ, mth, args, ...)  \
  typ mth args const { __VA_ARGS__ }

#define _mth_mut_inline(typ, mth, args, ...)  \
  typ mth args { __VA_ARGS__ }
#define _mth_mut(typ, mth, args)              \
  typ mth args;

#define _fry(typ, mth, args) \
  static typ mth args;

#define _cst(typ, name) \
  static _atr(typ, name)

// ... operators ...
#define _op(op)               \
  operator op const;
#define _op_mut(op)           \
  operator op;
#define _op_inline(op, expr)  \
  operator op const { return expr; }

// ... not covered by the above ...
#define _dcl(...)     __VA_ARGS__
#define _ns_dcl(...)  namespace NS___ { __VA_ARGS__ }

#else

#define _typedefs
#define _cpp_struct_typedefs
#define _cpp_struct
#define _cpp_struct_end
#define _con(...)
#define _xon(...)
#define _con_fwd(args, fwd)
#define _con_c_fwd(args, fwd)
#define _des()
#define _mth(typ, mth, args)
#define _mth_inline(typ, mth, args, ...)
#define _mth_mut(typ, mth, args)
#define _mth_mut_inline(typ, mth, args, ...)
#define _fry(typ, mth, args)
#define _cst(typ, name)
#define _op(op)
#define _op_inline(op, code)
#define _op_mut(op)
#define _dcl(...)
#define _ns_dcl(...)

#endif

//------------------------------------------------------------------------------
// cpp subclassing

#if _is_cpp_

#define _struct_pref(pref)     \
  namespace NS___ { pref struct DS___ { _cpp_struct_typedefs
#define _struct _struct_pref()
#define _struct_templ _struct_pref(template <typename T>)
#define _struct_end \
  };}

#define _iface     \
  _struct virtual ~DS___();
#define _iface_end \
  };}

#define _iface_mth_body(typ, mth, args, body) \
  virtual typ mth args const body
#define _iface_mth_mut_body(typ, mth, args, body) \
  virtual typ mth args body

#define _iface_mth(typ, mth, args)     _iface_mth_body(typ, mth, args, = 0;)
#define _iface_mth_mut(typ, mth, args) _iface_mth_mut_body(typ, mth, args, = 0;)

#define _struct_sub_pre_pre(pre,pre_s,s)  \
  namespace NS___ { pre struct DS___ : pre_s s { _cpp_struct_typedefs using base = s;

#define _struct_sub(s)       _struct_sub_pre_pre(,,s)
#define _struct_sub_templ(s) _struct_sub_pre_pre(template <typename T>,protected,s)
#define _struct_sub_end  };}

#define sub_struct(s, b) struct s : b { _typedefs(s) using base = b;
#define sub_struct_end };

#define sub_struct_reimpl(s, b) struct s : protected b { _typedefs(s) using base = b; \
  base const& base_rc() const { return *this; }

#else

#define _struct
#define _struct_end

#define _iface
#define _iface_end

#define _iface_mth_body(typ, mth, args, body)
#define _iface_mth_mut_body(typ, mth, args, body)
#define _iface_mth(typ, mth, args)
#define _iface_mth_mut(typ, mth, args)

#define _struct_sub(s)
#define _struct_sub_end

#endif

//------------------------------------------------------------------------------
// handy helpers

#if _is_cpp_

#define C_BASE_CONS using c_base::c_base;
#define BASE_CONS   using base::base;

#define NO_MOVE(s)              \
  s(rval)             = delete;   \
  ref operator=(rval) = delete;

#define NO_COPY(s)              \
  s(rc)               = delete; \
  ref operator=(rc)   = delete; \
  ref operator=(rval) = delete;

#define NO_GEN(s)               \
  s(rc)               = delete; \
  ref operator=(rc)   = delete; \
  s(rval)             = delete; \
  ref operator=(rval) = delete;

#define COMPARABLE              \
  int compare(rc)     const;
#define EQ_NE                   \
  bool operator==(rc) const;    \
  bool operator!=(rc) const;
#define LGTE                    \
  bool operator< (rc) const;    \
  bool operator<=(rc) const;    \
  bool operator> (rc) const;    \
  bool operator>=(rc) const;

#define COMP_OPS EQ_NE LGTE

#define _cpp_code(...) __VA_ARGS__
#define _cpp_private  private:

#else

#define NO_MOVE(s)
#define NO_COPY(s)
#define NO_GEN(s)
#define COMPARABLE
#define EQ_NE
#define LGTE
#define COMP_OPS

#define _cpp_code(...)
#define _cpp_private

#endif

#if _is_cpp_

// make a type mutable
template <typename T> struct mut_typ            { typedef T typ; };
template <typename T> struct mut_typ<T const>   { typedef T typ; };

// make a var mutable
template <typename T> T*& mut(T const*const& t) { return const_cast<T*&>(t); }
template <typename T> T&  mut(T const& t)       { return const_cast<T&> (t); }

#endif

//------------------------------------------------------------------------------
#endif
// eof
