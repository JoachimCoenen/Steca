/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: math.cpp                                                      $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/types/math.cpp 5     25/06/04 12:28p Ian                $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file  math.cpp
 */

#include "math.hpp"
#include <math.h>

namespace axmath {

//----------------------------------------------------------------------------

Constant::Constant(double value)
  : m_doubleValue(value), m_floatValue((float)value)
{
}


//----------------------------------------------------------------------------

Constant pi2    (AXMATH_PI/2),
         pi     (AXMATH_PI),
        _2pi    (AXMATH_PI*2),
        _4pi    (AXMATH_PI*4);

Constant c      (AXMATH_C),
         eps0   (AXMATH_EPS0),
         mu0    (AXMATH_MU0), 
         sigma0 (AXMATH_SIGMA0),
         rho0   (AXMATH_RHO0);

//----------------------------------------------------------------------------

} // namespace

//** eof **********************************************************************
