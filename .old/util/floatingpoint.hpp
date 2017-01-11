/******************************************************************************
* AxFDTD Accelerator; Copyright © 2004 Acceleware Inc.                        *
*                                                                             *
* $Workfile:: floatingpoint.hpp                                             $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/util/floatingpoint.hpp 2     6/10/04 4:12p Dan          $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file  floaintpoint.hpp
*  \brief Floating point control and analysis code
*/

#ifndef  UTIL_FLOATINGPOINT_HPP_INCLUDED
#define  UTIL_FLOAIINGPOINT_HPP_INCLUDED

#include <cfloat>

#include "../types/types.hpp"

namespace util {

    namespace FloatingPoint {   

        enum RoundingSSE { 
            SSE_RTZ = 0x6000,
            SSE_RU = 0x4000,
            SSE_RD = 0x2000,
            SSE_RN = 0x0000
        };

        enum RoundingX87 { 
            X87_RTZ = _RC_CHOP,
            X87_RU = _RC_UP,
            X87_RD = _RC_DOWN,
            X87_RN = _RC_NEAR
        };

        enum PrecisionX87 {
            X87_SINGLE = _PC_24,  // (24 bits) 
            X87_DOUBLE = _PC_53,  // (53 bits)
            X87_EXTENDED = _PC_64 // (64 bits)
        };

        // difference between a and b in units of the last place (ulp)
        // if signedZero is false, don't differentiate between +0.0 and -0.0
        int ulpDifference(float a, float b, bool signedZero = true);  

        // set SSE control register
        uint readControlRegSSE();
        void writeControlRegSSE(uint control);
        void setRoundingSSE(const RoundingSSE& rounding);

        // set X87 control register
        uint readControlRegX87();
        void writeControlRegX87(uint control);
        void setPrecisionX87(const PrecisionX87& precision);
        void setRoundingX87(const RoundingX87& rounding);

        /// Mask a float by a bit mask.
        inline void MaskFloat(float& target, uint mask)
        {
            (*reinterpret_cast<uint*>(&target)) &= mask;
        }
    
    } // namespace FloatingPoint

} // namespace util

#endif // UTIL_FLOATINGPOINT_HPP_INCLUDED

//** eof **********************************************************************
