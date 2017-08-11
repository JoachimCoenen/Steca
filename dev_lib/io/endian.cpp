// (dev_lib)

#include "endian.hpp"

namespace l_io { namespace endian {
//------------------------------------------------------------------------------

uint16 from_be(uint16 v) {
  return v; // be16toh(v); TODO
}

uint16 from_le(uint16 v) {
  return v; // le16toh(v);
}

uint32 from_be(uint32 v) {
  return v; // be32toh(v);
}

uint32 from_le(uint32 v) {
  return v; // le32toh(v);
}

uint64 from_be(uint64 v) {
  return v; // be64toh(v);
}

uint64 from_le(uint64 v) {
  return v; // le64toh(v);
}

//------------------------------------------------------------------------------
}}
// eof
