// c

#ifndef C_CPP_APP_I_H
#define C_CPP_APP_I_H

#include <c/c/h>

_c_cpp_iface(app)
  virtual ~app();
  virtual int exec() = 0;
_c_cpp_iface_end

#endif
// eof
