/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: registry.hpp                                                  $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/util/registry.hpp 1     15/07/04 3:12p Ian              $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file  registry.hpp
 *  \brief WIndows Registry
 */

#ifndef UTIL_REGISTRY_HPP_INCLUDED
#define UTIL_REGISTRY_HPP_INCLUDED

#include "../types/types.hpp"

namespace util {

//----------------------------------------------------------------------------

  class RegistryKey
  {
  public:
    typedef pvoid rkey;

    RegistryKey();
    RegistryKey(rkey root,bool write,pcstr subkey = NULL);
    RegistryKey(RegistryKey const&);
    RegistryKey& operator= (RegistryKey const&);

    virtual ~RegistryKey();

    bool Open(rkey root,bool write,pcstr subkey);
    bool OpenSubkey(bool write,pcstr subkey);

    void Close();

  public:
    bool IsOpen() const
    { 
        return NULL!=m_rKey;
    }

    rkey RKey() const { return m_rKey;  }

    bool ValueExists(pcstr value) const;
    bool AnyValueExist()          const;

    bool QueryValue(pcstr value,std::string& data)   const;
    bool QueryValue(pcstr value,uint& data) const;

    bool CreateSubkey(pcstr subkey,bool open=false);

    bool CreateValue(pcstr value,pcstr data);
    bool CreateValue(pcstr value,uint data);

    bool IsEmpty();

    bool RemoveValue(pcstr value);
    bool RemoveSubkey(pcstr subkey);
    bool RemoveSubtree(pcstr subkey);

  private:
    rkey m_rKey;
    bool m_write;
  };

//-----------------------------------------------------------------------------

} // namespace

#endif

//** eof **********************************************************************
