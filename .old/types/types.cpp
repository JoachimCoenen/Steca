/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: types.cpp                                                     $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/types/types.cpp 4     19/07/04 1:12p Ian                $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file  types.cpp
 */

  #include "types.hpp"
  #include <windows.h>
  #include <stdarg.h>
  #include <string>

//----------------------------------------------------------------------------

  std::string exc::What() const
  {
    return sformat("Exception %u (\"%s\")",Id(),Msg().c_str());
  }

//----------------------------------------------------------------------------

  std::string sformat(pcstr format,...)
  {
    pstr txt = NULL;

    try
    {
      size_t SIZE = 1024;  // initial size

      for(;;)
      {
        delete [] txt;
        txt = new char[SIZE+1];

        va_list marker;
        va_start(marker,format);

        int cnt = _vsnprintf(txt,SIZE,format,marker);

        va_end(marker);

        if(cnt<0)
        {
          SIZE *= 2;    // try again with a bigger buffer
        }
        else
        {
          txt[cnt] = 0; // if cnt==SIZE, _vsnprintf does not write '\0'
          break;
        }
      }
    }
    catch(...)
    {
      delete [] txt;
      throw;
    }

    std::string s(txt);
    delete [] txt;

    return s;
  }

  static pcstr whiteSpace = " \n\t";

  std::string trimleft(std::string const& s)
  {
    std::string::size_type f = s.find_first_not_of(whiteSpace);
    if(std::string::npos==f)
      return std::string();
    else
      return s.substr(f);
  }
  
  std::string trimright(std::string const& s)
  {
    std::string::size_type l = s.find_last_not_of(whiteSpace);
    if(std::string::npos==l)
      return std::string();
    return s.substr(0,l+1);
  }
  
  std::string trim(std::string const& s)
  {
    std::string::size_type f = s.find_first_not_of(whiteSpace);
    if(std::string::npos==f)
      return std::string();

    std::string::size_type l = s.find_last_not_of(whiteSpace);
    return s.substr(f,l+1-f);
  }

/*** eof ********************************************************************/
