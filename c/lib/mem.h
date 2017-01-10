// c

#ifndef C_MEM_H
#define C_MEM_H

#include "../def/def_num"

C_STRUCT(mem, {
  size_t size;
  void   *p;

  C_CON(mem, (size_t, void*))
})

C_STRUCT_CPP(mem, { NO_COPY(mem)
  mem(size_t);
  mem(size_t, void const* src);
  mem(rval);
 ~mem();
protected:
  void swap(ref);
})

//------------------------------------------------------------------------------
#endif
// eof
