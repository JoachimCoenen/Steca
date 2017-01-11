/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: registry.cpp                                                  $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/util/registry.cpp 2     17/08/04 2:44p Ian              $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file registry.cpp
 */

#include "registry.hpp"
#include "array.hpp"
#include <windows.h>

namespace util {

//----------------------------------------------------------------------------

RegistryKey::RegistryKey()
  : m_rKey(NULL), m_write(false)
{
}


RegistryKey::RegistryKey(rkey root,bool write,pcstr subkey)
  : m_rKey(NULL), m_write(false)
{
    Open(root,write,subkey);
}


RegistryKey::RegistryKey(RegistryKey const& r)
  : m_rKey(NULL), m_write(false)
{
    *this = r;
}


RegistryKey& RegistryKey::operator= (RegistryKey const& r)
{
    Close();
    if(r.IsOpen()) Open(r.m_rKey,r.m_write,NULL);
    return *this;
}


RegistryKey::~RegistryKey()
{
    Close();
}


static DWORD KeyAccess(bool write)
{
    return write
        ? KEY_WRITE|KEY_ENUMERATE_SUB_KEYS|KEY_QUERY_VALUE
        : KEY_READ|KEY_QUERY_VALUE;
}


bool RegistryKey::Open(rkey root,bool write,pcstr subkey)
{
    Close();

    rkey rKey;
    if(ERROR_SUCCESS==RegOpenKeyEx((HKEY)root,subkey,0,KeyAccess(write),(PHKEY)&rKey))
    {
        m_write = write;
        m_rKey  = rKey;
        return true;
    }

    return false;
}


bool RegistryKey::OpenSubkey(bool write,pcstr subkey)
{
    if(!IsOpen()) return false;

    rkey rKey;
    if(ERROR_SUCCESS==RegOpenKeyEx((HKEY)m_rKey,subkey,0,KeyAccess(write),(PHKEY)&rKey))
    {
        RegCloseKey((HKEY)m_rKey);
        m_write = write;
        m_rKey  = rKey;
        return true;
    }

    Close();
    return false;
}


void RegistryKey::Close()
{
    if(IsOpen()) RegCloseKey((HKEY)m_rKey);
    m_rKey  = NULL;
    m_write = false;
}


bool RegistryKey::ValueExists(pcstr value) const
{
    if(!IsOpen()) return false;

    return (bool)
        (ERROR_SUCCESS==RegQueryValueEx((HKEY)m_rKey,value,NULL,NULL,NULL,NULL));
}


bool RegistryKey::AnyValueExist() const
{
    if(!IsOpen()) return false;

    DWORD buflen = 0;   // no buffer (cheating)

    return (bool)
        (ERROR_SUCCESS==RegEnumValue((HKEY)m_rKey,0,NULL/*value name buffer*/,&buflen,NULL,NULL,NULL,NULL));
}


bool RegistryKey::QueryValue(pcstr value,std::string& data) const
{
    if(!IsOpen()) return false;

    // first get the value type and size
    DWORD type;
    DWORD size = 0;
    if(ERROR_SUCCESS==RegQueryValueEx((HKEY)m_rKey,value,NULL,&type,NULL,&size)
       && REG_SZ==type)
    {
        // get the value
        array1d<unsigned char> a(size+1);
        if(ERROR_SUCCESS==RegQueryValueEx((HKEY)m_rKey,value,NULL,NULL,a.ptr(),&size))
        {
            a[size] = 0;
            data    = (char*)a.ptr();
            return true;
        }
    }

    return false;
}


bool RegistryKey::QueryValue(pcstr value,uint& data) const
{
    if(!IsOpen()) return false;

    // get the value data and type 
    DWORD dw;
    DWORD type;
    DWORD size = sizeof(dw);
    if(ERROR_SUCCESS==RegQueryValueEx((HKEY)m_rKey,value,NULL,&type,(PBYTE)&dw,&size)
       && REG_DWORD==type)
    {
        data   = dw;
        return true;
    }

    return false;
}


bool RegistryKey::CreateSubkey(pcstr subkey,bool open)
{
    if(!IsOpen() || ::empty(subkey)) return false;

    rkey  hSubkey;
    DWORD disposition;
    if(ERROR_SUCCESS==RegCreateKeyEx((HKEY)m_rKey,subkey,0,"",REG_OPTION_NON_VOLATILE,
                    KEY_WRITE,NULL,(PHKEY)&hSubkey,&disposition))
    {
        RegCloseKey((HKEY)hSubkey);
        m_write = true;
        return open ? OpenSubkey(true,subkey) : true;
    }

    return false;
}


bool RegistryKey::CreateValue(pcstr value,pcstr data)
{
    if(!IsOpen()) return false;

    std::string s(data);
    if(ERROR_SUCCESS==RegSetValueEx((HKEY)m_rKey,value,0,REG_SZ,
        (PBYTE)s.c_str(),s.size()+1))
    {
        return true;
    }

    return false;
}


bool RegistryKey::CreateValue(pcstr value,uint data)
{
    if(!IsOpen()) return false;

    if(ERROR_SUCCESS==RegSetValueEx((HKEY)m_rKey,value,0,REG_DWORD,
        (PBYTE)&data,sizeof(data)))
    {
        return true;
    }

    return false;
}


bool RegistryKey::IsEmpty()
{
    if(!IsOpen()) return true;

    char  name[128];
    DWORD nameSize = sizeof(name);
    FILETIME ft;

    if(ERROR_SUCCESS!=RegEnumKeyEx((HKEY)m_rKey,0,name,&nameSize,NULL,NULL,NULL,&ft))
    {
        nameSize = sizeof(name);
        // there are no subkeys, check for any values
        if(ERROR_SUCCESS!=RegEnumValue((HKEY)m_rKey,0,name,&nameSize,NULL,NULL,NULL,NULL))
        {
            // no values, is empty
            return true;
        }
    }

    return false;
}


bool RegistryKey::RemoveValue(pcstr value)
{
    if(!IsOpen()) return false;

    RegDeleteValue((HKEY)m_rKey,value);
    // check the nonexistence
    return (bool) !ValueExists(value);
}


bool RegistryKey::RemoveSubkey(pcstr subkey)
{
    if(!IsOpen() || ::empty(subkey)) return false;

    {  // NT-95 difference, check if empty
        RegistryKey k(*this);
        if(!k.OpenSubkey(false,subkey)) 
            return true;   // subkey does not exist
        if(!k.IsEmpty()) 
            return false;   // exists and is not empty
    }

    RegDeleteKey((HKEY)m_rKey,subkey);

    {  // check the nonexistence
        RegistryKey k(*this);
        return (bool) !k.OpenSubkey(false,subkey); // should not exist now
    }
}


bool RegistryKey::RemoveSubtree(pcstr subkey)
{
    if(!IsOpen() || ::empty(subkey)) return false;

    // NT-95 difference, enumerate and recurse
    {
        RegistryKey k(*this);
        if(!k.OpenSubkey(false,subkey)) 
            return true;   // subkey does not exist

        k = *this;  // again
        if(!k.OpenSubkey(true,subkey)) 
            return false;   // cannot write

        for(;;)     // keys
        {
		    char  subName[128/*magic number*/];
            DWORD subNameSize = sizeof(subName);

            LONG result = RegEnumKeyEx((HKEY)k.RKey(),0/*always the 1st key*/,
                                subName,&subNameSize,
                                NULL,NULL,NULL,NULL);

            // if there is a subkey, delete it recursively and try again ..
            if(ERROR_SUCCESS==result)
            {
                RegistryKey k1 = k;
                if(!k1.RemoveSubtree(subName)) 
                    return false;

                continue;
            }

            // .. until there is no more subkeys
            if(ERROR_NO_MORE_ITEMS==result)
                break;

            // another error, fail (or ignore and delete as much as possible?)
            return false;
        }
        
        for(;;)     // values
        {
		    char  valueName[128/*magic number*/];
            DWORD valueNameSize = sizeof(valueName);

            LONG result = RegEnumValue((HKEY)k.RKey(),0/*always the 1st key*/,
                                valueName,&valueNameSize,
                                NULL,NULL,NULL,NULL);

            // if there is a value, delete it
            if(ERROR_SUCCESS==result && k.RemoveValue(valueName))
                continue;

            // .. until there is no more values
            if(ERROR_NO_MORE_ITEMS==result)
                break;

            // another error, fail (or ignore and delete as much as possible?)
            return false;
        }

    }

    // now, all subs should be removed
    return RemoveSubkey(subkey);
}

//-----------------------------------------------------------------------------

} // namespace

//** eof **********************************************************************
