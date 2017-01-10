// c

#ifndef C_APP_H
#define C_APP_H

#include "../def/def_h"

C_IFACE_CPP(app, {
  virtual ~app();
  virtual int exec() = 0;
})

#endif
// eof
