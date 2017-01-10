// c

#ifndef C_STR_H
#define C_STR_H

#include "mem.h"

typedef c_mem c_str;

typedef char*       pstr;
typedef char const* pcstr;

C_SUB_STRUCT_CPP(str, mem, { BASE(mem)
  str(pcstr);
  str(rval);

  operator pcstr() const {
    return static_cast<pcstr>(p);
  }
})

//------------------------------------------------------------------------------
#endif
// eof
