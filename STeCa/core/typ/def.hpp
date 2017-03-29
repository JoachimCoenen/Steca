// (c2)

#ifndef CORE_DEF_HPP
#define CORE_DEF_HPP

#include <c2/inc/c_def.h>
#include <c2/c/ij.h>
#include <c2/c/num.h>
#include <c2/c/trg.h>
#include <c2/cpp/exc.hpp>
#include <c2/cpp/vec.hpp>

namespace core {
//------------------------------------------------------------------------------

typedef c::vec<real> real_vec;
typedef c::vec<uint> uint_vec;

typedef flt32 inten_t;
typedef c::vec<inten_t> inten_vec;

typedef c::deg tth_t;
typedef c::deg gma_t;

struct  Range;
typedef Range tth_rge;
typedef Range gma_rge;

typedef flt32  fwhm_t;

//------------------------------------------------------------------------------
}
#endif
// eof