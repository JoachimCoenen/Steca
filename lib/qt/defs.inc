// (lib/qt)

/**@file
 * Include this file, directly or indirectly, into each *source* file
 * that uses the (with qt) library.
*/

#ifdef QT_LIB_DEFS_INC
  #error inlude only once
#endif
#define QT_LIB_DEFS_INC

#include <lib/dev/inc/defs.inc>
#include "qstr.inc"

//------------------------------------------------------------------------------
// eof
