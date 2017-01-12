// c

#ifndef C_C_APP_I_H
#define C_C_APP_I_H

#include "../def/def_h"

_c_cpp_iface(app)
  virtual ~app();
  virtual int exec() = 0;
_c_cpp_iface_end

#endif
// eof
