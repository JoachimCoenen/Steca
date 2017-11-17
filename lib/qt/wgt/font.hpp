// (lib/qt)

/** @file
 * font metrics
 */
#pragma once

#include "../defs.hpp"

class QWidget;

namespace l_qt {
//------------------------------------------------------------------------------

/// width in 'm's
int width_m(QWidget const&, real n = 1);

/// width in '0's
int width_0(QWidget const&, real n = 1);

//------------------------------------------------------------------------------
}
// eof
