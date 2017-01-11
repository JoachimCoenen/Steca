/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: types.h                                                       $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/types/types.h 4     14/07/04 2:06p Ian                  $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file  types.h
 *  \brief Basic "C" types and typedefs.
 *
 *  Common include file that should be included everywhere.
 */

#ifndef AX_TYPES_H_INCLUDED
#define AX_TYPES_H_INCLUDED

/*---------------------------------------------------------------------------*/

#ifdef _MSC_VER
  #pragma warning(disable: 4290)  // C++ Exception Specification ignored
  #pragma warning(disable: 4355)  // 'this' used in base member initializer list
  #pragma warning(disable: 4786)  // identifier was truncated (templates)
  #pragma warning(disable: 4503)  // decorated name length exceeded
  #pragma warning(disable: 4097)  // typedef used as synonym for class-name
#endif

#ifdef __cplusplus
  #include <cassert>
  #include <string>
  extern "C" {
#else
  #include <assert.h>
#endif

/*---------------------------------------------------------------------------*/
/** Basic types                                                              */

  #ifndef __cplusplus
    #define inline __inline     /**< inline keyword */
  #endif

  typedef void*          pvoid;  /**< void pointer */
  typedef void const*    pcvoid; /**< void const pointer */
                        
  #ifndef NULL
    #define NULL ((pvoid)0)     /**< null pointer */
  #endif

  typedef char*          pstr;   /**< pointer to a C-type string */
  typedef char const*    pcstr;  /**< pointer to a constant C-type string */
  
  typedef unsigned char  byte;   /**< unsigned char */
  typedef byte*          pbyte;  /**< pointer to a byte C-type string */
  typedef byte const*    pcbyte; /**< pointer to a constant byte C-type string */

  typedef unsigned short ushort; /**< unsigned short integer */
  typedef unsigned int   uint;   /**< unsigned integer */
  typedef unsigned long  ulong;  /**< unsigned long */

#ifdef _MSC_VER
  typedef __int64          int64;   /**< signed long long  */
  typedef unsigned __int64 uint64;  /**< unsigned long long */
#endif

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
  } /* extern "C" */
#endif

#endif

/*** eof *********************************************************************/
