/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: floatingpoint.cpp                                             $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/util/floatingpoint.cpp 2     6/10/04 4:12p Dan          $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file  floatingpoint.cpp
*  \brief Floating point control and analysis code
*
*/

#include "floatingpoint.hpp"
#include <xmmintrin.h>

namespace util {

    namespace FloatingPoint {

        int ulpDifference(float a, float b, bool signedZero)
        {
            const int SIGN_MASK = 0x80000000;
            if ( a == b ) return 0;
            int diff = *(int*)&a - *(int*)&b;
            if ( !signedZero && (diff == SIGN_MASK) )  // if signedZero is not supported, return 0 ulp error
                return 0;
            return diff;
        }

        uint readControlRegSSE()
        {
            return _mm_getcsr();
        }

        void writeControlRegSSE(uint control)
        {
            _mm_setcsr(control);
        }

        void setRoundingSSE(const RoundingSSE& rounding)
        {
            uint oldMode =_mm_getcsr();
            _mm_setcsr( oldMode | rounding );
        }

        uint readControlRegX87()
        {
            return _controlfp(0,0);
        }

        void writeControlRegX87(uint control)
        {
            _controlfp( control, 0xffffffff );
        }

        void setRoundingX87(const RoundingX87& rounding)
        {
            _controlfp( rounding, _MCW_RC );
        }

        void setPrecisionX87(const PrecisionX87& precision)
        {
            _controlfp( precision, _MCW_PC );
        }

    } // namespace FloatingPoint

} // namespace util

//** eof **********************************************************************


