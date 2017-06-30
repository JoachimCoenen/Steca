// (app_lib)

#ifndef CORE_DEF_HPP
#define CORE_DEF_HPP

#include <dev_lib/inc/defs.hpp>
#include <dev_lib/typ/trg.hpp>
#include <dev_lib/typ/vec.hpp>

namespace core {
//------------------------------------------------------------------------------

using real_vec = l::vec<real>;
using uint_vec = l::vec<uint>;

using tth_t = l::deg;
using omg_t = l::deg;
using phi_t = l::deg;
using chi_t = l::deg;

using gma_t = l::deg;

struct Range;

using tth_rge = Range;
using gma_rge = Range;

using peak_t = flt32;
using fwhm_t = flt32;

using inten_t   = flt32;
using inten_vec = l::vec<inten_t>;
using inten_rge = Range;

//------------------------------------------------------------------------------
}

#endif
// eof
