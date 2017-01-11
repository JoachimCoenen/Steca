// c

#ifndef C_APP_H
#define C_APP_H

#include "../def/def_h"

_c_cpp_iface(app)
  virtual ~app();
  virtual int exec() = 0;
_c_cpp_iface_end(app)

#endif
// eof
