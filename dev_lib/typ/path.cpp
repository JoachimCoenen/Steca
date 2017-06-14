// (dev_lib)

#include "path.h"

#ifdef _WIN32
  #include <filesystem>
#else
  #include <string.h>
#endif

namespace l {
//------------------------------------------------------------------------------

path::path(strc s) : base(s) {}

str path::basename() const {
#ifdef _WIN32
  return str(std::tr2::sys::path(p).stem().generic_string().c_str());
#else
  return str(::basename(c_str()));
#endif
}

//------------------------------------------------------------------------------
}
// eof