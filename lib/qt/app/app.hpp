// (lib/qt)

/** @file
 * application
 */

#pragma once

#include "../defs.hpp"
#include <QApplication>

namespace l_qt {
//------------------------------------------------------------------------------

struct win;

dcl_sub_(app, QApplication)
  app(int& argc, char* argv[]);

  /// show a window; execute the message loop
  int exec(win* = nullptr);
  /// same as @c exec, but also catch exceptions and return orderly
  int safeExec(win* = nullptr);

  /// a helper that suppresses warning for its lifetime
  dcl_(NoWarnings)
    NoWarnings();
   ~NoWarnings();
  dcl_end

private:
  bool notify(QObject*, QEvent*);
dcl_end

//------------------------------------------------------------------------------
}
// eof
