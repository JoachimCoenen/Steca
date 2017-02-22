// (c)

#ifndef C_C_STR_H
#define C_C_STR_H

#include "../c_def.h"

typedef char*       pstr;
typedef char const* pcstr;

#define NS___ c
#define DS___ str

_c_struct
  _atr(sz_t, sz)
  _ptr(char, p)

  _c_con(sz_t, pcstr)
_c_struct_end

_cpp_struct COMPARABLE COMP_OPS
  _con()
  _con(pcstr)
  _con(sz_t maxSz, pcstr)
  _con(rc)
  _con(rval)
  _des()

  _mth(bool, empty, ())
  _mth(bool, eq,    (pcstr))

  _mth_mut(rc, set, (pcstr))
  _mth_mut(rc, set, (rval))

  _op_inline (pcstr(), p)

  _mth(str, trim, ())
  _fry(str, cat,  (pcstr, pcstr))

  _cst(str, nul)
_cpp_struct_end

_cpp_code(typedef c::str::rc strc;)

_cpp_code(
namespace c { namespace unsafe {
str str_frm(pcstr, ...);
str str_cat(pcstr, ...);
}})

#undef DS___
#undef NS___

#endif
// eof
