// (lib/qt)

/**@file
 * Main window.
 */

#pragma once

#include "../defs.hpp"
#include <QMainWindow>

namespace l_qt {
//------------------------------------------------------------------------------

struct acts;
struct split;
struct panel;

/// the main window
dcl_sub_(win, QMainWindow)
  win();

  mth_mut_(split&, hs, ());         ///< make a horizontal split the central widget
  mth_mut_(split&, vs, ());         ///< make a vertical split the central widget
  mth_mut_(panel&, makePanel, ());  ///< make a panel the central widget

  /// obtain actions
  virtual mth_(acts const&, getActs, ()) = 0;

  virtual mut_(show,  (bool = true)); ///< show and restore the saved geometry
  virtual mut_(fullScreen, (bool));   ///< fullscreen on / off
  virtual mut_(about, ()) {}          ///< provide an about dialog, if wished
  virtual mut_(quit,  ());            ///< handle quit request

  virtual mut_(onFirstShow, ()) {}    ///< what to do when first shown
  virtual bol_mut_(onClose,     ()) RET_(true)  ///< return @c false to prevent closing

protected:
  void closeEvent(QCloseEvent*);
  bool firstShow;
  QByteArray initialState;
dcl_end

//------------------------------------------------------------------------------
}
// eof
