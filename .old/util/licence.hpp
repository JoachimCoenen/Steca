/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: licence.hpp                                                   $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/util/licence.hpp 1     15/07/04 3:12p Ian               $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file  licence.hpp
 *  \brief MD5 licence
 */

#ifndef UTIL_LICENCE_HPP_INCLUDED
#define UTIL_LICENCE_HPP_INCLUDED

#include "../types/types.hpp"

namespace util {

//----------------------------------------------------------------------------

  namespace licence
  {
    typedef std::basic_string<byte> memory;

    std::string MakeKey(pcstr productId,uint licNo,pcstr licName);
    bool Verify(pcstr productId,uint licNo,pcstr licName,pcstr licKey);
  }

//-----------------------------------------------------------------------------

} // namespace

#endif

//** eof **********************************************************************
