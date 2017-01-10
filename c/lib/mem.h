// c

#ifndef C_MEM_H
#define C_MEM_H

#include "../def/def_num"

C_STRUCT_C(mem)
  C_ATR_C(size_t, size)
  C_PTR_C(void,   p)

  C_CON_C(mem, (size_t, void*))
C_STRUCT_C_END(mem)

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
