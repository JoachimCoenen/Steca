// (dev_lib)

#include "str.hpp"
#include "defs.inc"

//------------------------------------------------------------------------------

flt32 str::asFlt() const may_err {
  guard_err_("str::asFlt",
    return std::stof(*this);
  );
}

namespace std {

size_t hash<::str>::operator()(::strc s) const {
  return hash<string>{}(s);
}

}

str const str::null;

//------------------------------------------------------------------------------
// eof
