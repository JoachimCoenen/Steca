// (lib/dev)

#include "str.hpp"
#include "defs.inc"
#include <algorithm>

//------------------------------------------------------------------------------

flt32 str::asFlt() const may_err {
  wrap_err_msg_("str::asFlt",
    return std::stof(*this);
  );
}

str str::trimmed() const {
  pcstr p1 = c_str(), p2 = p1, p3 = p1 + length();
  while (p2 < p3 && *p2)                            // find the end
    ++p2;
  while (p1 < p2 && std::isspace(*p1))              // trim left
    ++p1;
  while (p1 < p2 && std::isspace(*(p2-1)))          // trim right
    --p2;
  return str(p1, l::to_u(p2-p1));                   // substring
}

str str::lower() const {
  str s(*this);
  std::transform(s.begin(), s.end(), s.begin(), ::tolower);
  return s;
}

str str::num(int val) {
  return CAT(val);
}

str str::num(uint val) {
  return CAT(val);
}

/// @cond
namespace std {

size_t hash<::str>::operator()(::strc s) const {
  return hash<string>{}(s);
}

}
/// @endcond

str const str::null;

//------------------------------------------------------------------------------
// eof