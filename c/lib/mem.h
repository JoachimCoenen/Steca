// c

#ifndef C_MEM_H
#define C_MEM_H

#include "../def/num.h"

C_STRUCT(mem, {
  typedef uint  size_t;

  size_t size;
  void   *p;

  C_CON(mem, (size_t, void*))
})

C_STRUCT_CPP(mem, { NO_COPY(mem)
  mem(size_t);
  mem(rval);
 ~mem();
})

//------------------------------------------------------------------------------
#endif
// eof
