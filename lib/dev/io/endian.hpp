// (lib/dev)

/** @file
 * conversion from big and little endians to the system representation;
 * TODO so far this is only an interface
 */

#pragma once

#include "../defs.hpp"

namespace l_io { namespace endian {
//------------------------------------------------------------------------------

enum ness { le, be };

uint16 from_be(uint16);
uint16 from_le(uint16);

uint32 from_be(uint32);
uint32 from_le(uint32);

uint64 from_be(uint64);
uint64 from_le(uint64);

//------------------------------------------------------------------------------
}}
// eof
