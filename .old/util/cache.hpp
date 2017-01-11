/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: cache.hpp                                                     $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/util/cache.hpp 1     12/07/04 12:37p Peter              $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file  cache.hpp
 *  \brief CPU cache info
 */

#ifndef UTIL_CACHE_HPP_INCLUDED
#define UTIL_CACHE_HPP_INCLUDED

#include "../types/types.hpp"

namespace util {

    //----------------------------------------------------------------------------

    namespace cache
    {
        uint AliasSize();
        uint MaxOffset();
    }
    //----------------------------------------------------------------------------

} // namespace

#endif

//** eof **********************************************************************
