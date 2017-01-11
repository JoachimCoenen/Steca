/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: math.hpp                                                      $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/types/math.hpp 6     25/06/04 12:28p Ian                $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file  math.hpp
 *  \brief Constants, and perhaps more.
 *
 */

#ifndef AX_MATH_HPP_INCLUDED
#define AX_MATH_HPP_INCLUDED

#include "types.hpp"

//-----------------------------------------------------------------------------

/// Ludolf's number
#define AXMATH_PI       (3.14159265358979323846)

/// speed of light
#define AXMATH_C        (2.99792458e8)

/// permittivity of free space
#define AXMATH_EPS0     (8.854187817e-12)
/// permeability of free space
#define AXMATH_MU0      (AXMATH_PI*4e-7)
/// electric conductivity
#define AXMATH_SIGMA0   (0.0)
/// magnetic conductivity
#define AXMATH_RHO0     (0.0)

namespace axmath {

//-----------------------------------------------------------------------------

/// Stores a floating point constant. Provides conversion.
class Constant
{
public:
    Constant(double value);

    inline operator float()  const  { return m_floatValue;  }
    inline operator double() const  { return m_doubleValue; }

private:
    double m_doubleValue;
    float  m_floatValue;
};

/// Pi/2, Pi, 2*Pi, 4*Pi.
extern Constant pi2,
                pi, 
               _2pi, 
               _4pi;

/// speed of light, permittivity, permeability, conductivities - of free space
extern Constant c,
                eps0,
                mu0, 
                sigma0,
                rho0;

//-----------------------------------------------------------------------------

} // namespace

#endif

//** eof **********************************************************************
