// (dev_lib)

#pragma once

#include "../defs.hpp"

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
  busy();
 ~busy();

  typedef void (*handler_t)(bool);

  static void set(handler_t);
  static void unset();
};

struct progress {
  typedef void (*handler_t)(uint val, uint total);

  static void step(uint val, uint total);

  static void set(handler_t);
  static void unset();
};

//------------------------------------------------------------------------------
}
// eof
