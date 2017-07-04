// (dev_lib)

#include "endian.hpp"
#include "../defs.inc"
#include <endian.h>

namespace l_io { namespace endian {
//------------------------------------------------------------------------------

uint16 from_be(uint16 v) {
  return be16toh(v);
}

uint16 from_le(uint16 v) {
  return le16toh(v);
}

uint32 from_be(uint32 v) {
  return be32toh(v);
}

uint32 from_le(uint32 v) {
  return le32toh(v);
}

uint64 from_be(uint64 v) {
  return be64toh(v);
}

uint64 from_le(uint64 v) {
  return le64toh(v);
}

//------------------------------------------------------------------------------
}}
// eof
