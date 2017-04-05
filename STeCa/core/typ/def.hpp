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

using real_vec = c::vec<real>;
using uint_vec = c::vec<uint>;

using tth_t = c::deg;
using omg_t = c::deg;
using phi_t = c::deg;
using chi_t = c::deg;

using gma_t = c::deg;

struct Range;

using tth_rge = Range;
using gma_rge = Range;

using fwhm_t = flt32;

using inten_t   = flt32;
using inten_vec = c::vec<inten_t>;

//------------------------------------------------------------------------------
}
#endif
// eof
