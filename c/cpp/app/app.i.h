// c

#ifndef C_CPP_APP_I_H
#define C_CPP_APP_I_H

#include <c/c/def/def_data>

#define DATA_NS    c
#define DATA_NAME  app

_iface
  _iface_mth_mut(int, exec, ())
_iface_end

#undef DATA_NS
#undef DATA_NAME

#endif
// eof
