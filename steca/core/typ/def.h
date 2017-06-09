// (app_lib)

#ifndef CORE_DEF_HPP
#define CORE_DEF_HPP

#include <app_lib/inc/defs_h.h>
#include <app_lib/typ/trg.h>
#include <QVector>
#include <QSharedPointer>

namespace core {
//------------------------------------------------------------------------------

using real_vec = QVector<real>;
using uint_vec = QVector<uint>;

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
using inten_vec = QVector<inten_t>;
using inten_rge = Range;

//------------------------------------------------------------------------------
}

#define SHARED \
  using sh  = QSharedPointer<Self>; \
  using shc = sh const&;

#endif
// eof
