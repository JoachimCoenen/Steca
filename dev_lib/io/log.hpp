// (dev_lib)

#pragma once

#include <dev_lib/defs.hpp>

namespace l_io {
//------------------------------------------------------------------------------

struct log {
  enum eType { INFO, WARN, MODAL };

  static void info(strc);
  static void warn(strc);
  static void modal(strc);

  typedef void (*handler_t)(strc, eType);

  static void set(handler_t);
  static void unset();
};

struct busy {
  typedef void (*handler_t)(bool);

  static void set(handler_t);
  static void unset();

  busy();
 ~busy();
};

//------------------------------------------------------------------------------
}
// eof
