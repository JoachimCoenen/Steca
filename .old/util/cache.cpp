/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: cache.cpp                                                     $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/util/cache.cpp 2     23/08/04 11:11a Peter              $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file cache.cpp
*/

#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
#include "cache.hpp"
#include <windows.h>

namespace util
{
//----------------------------------------------------------------------------
    namespace cache
    {
//----------------------------------------------------------------------------
        uint AliasSize()
        {
            __try {
                _asm {
                    xor eax, eax
                        cpuid
                }
            }
            __except (EXCEPTION_EXECUTE_HANDLER) {
                // CPU does not support cpuid exception...
                return 0;
            }

            DWORD standard = 0;
            DWORD feature = 0;
            DWORD max = 0;
            union {
                char cBuf[12+1];
                struct {
                    DWORD dw0;
                    DWORD dw1;
                    DWORD dw2;
                } s;
            } Ident;


            _asm {
                push ebx
                    push ecx
                    push edx

                    // get the vendor string
                    xor eax, eax
                    cpuid
                    mov max, eax
                    mov Ident.s.dw0, ebx
                    mov Ident.s.dw1, edx
                    mov Ident.s.dw2, ecx

                    // get the Standard bits
                    mov eax, 1
                    cpuid
                    mov standard, eax
                    mov feature, edx

                    pop edx //dc>>> should this be the reverse order of the push instructions to restore register contents?
                    pop ecx
                    pop ebx
            }

            int family, model, stepping;

            family = (standard >> 8) & 0xF;   // retrieve family
            if (family == 15) {               // retrieve extended family
                family |= (standard >> 16) & 0xFF0;
            }
            model = (standard >> 4) & 0xF;    // retrieve model
            if (model == 15) {                // retrieve extended model
                model |= (standard >> 12) & 0xF;
            }
            stepping = (standard) & 0xF;      // retrieve stepping

            // dc>>> Can different manufacturers use the same family numbers?
            // If so, I need to check the vendor string as well as the family and model info...
            if ( family == 15 && model == 2 ) // P4 Northwood
                return 0x10000;
            if ( family == 15 && model == 3 ) // P4 Prescott
                return 0x100000;

            // unknown processor
            assert( 0 ); // can't use ASSERT_EXC because unwinding doesn't work within __try
            return 0; //dc>>>We don't know anything about the cache!

        }

        uint MaxOffset()
        {
            uint aliasSize = AliasSize();
            if(aliasSize>1)
            {
                return (aliasSize << 1) - 1;
            }
            return 0;
        }

    } // namespace cache
//-----------------------------------------------------------------------------

} // namespace util

//** eof **********************************************************************
