// (dev_lib)

#pragma once

#include <dev_lib/defs.hpp>

namespace l_io { namespace endian {
//------------------------------------------------------------------------------

uint16 from_be(uint16);
uint16 from_le(uint16);

uint32 from_be(uint32);
uint32 from_le(uint32);

uint64 from_be(uint64);
uint64 from_le(uint64);

//------------------------------------------------------------------------------
}}
// eof
