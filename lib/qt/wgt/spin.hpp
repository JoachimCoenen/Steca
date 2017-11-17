// (lib/qt)

/** @file
 * an extended Qt widget
 */
#pragma once

#include "../defs.hpp"
#include <lib/dev/inc/num.hpp>
#include <QSpinBox>

namespace l_qt {
//------------------------------------------------------------------------------

/// spinbox
dcl_sub_(spin, QSpinBox)
  /// @c digitWidth: a number of digits without scrolling (approximate)
  explicit spin(uint digitWidth);

  /// set the width as a number of digits
  set_(digitWidth, (uint));

  set_(min, (int));       ///< set the minimum
  set_(max, (int));       ///< set the maximum
  set_(val, (int));       ///< set the value
  mth_(int, val, ());     ///< get the value
dcl_end

/// integer spinbox
dcl_sub_(spinInt, spin)
  spinInt(uint digitWidth);

signals:
  void valChg(int);       ///< the value has changed
private:
  Q_OBJECT
dcl_end

/// unsigned integer spinbox
dcl_sub_(spinUint, spin)
  explicit spinUint(uint digitWidth);

signals:
  void valChg(uint);      ///< the value has changed
private:
  Q_OBJECT
dcl_end

/// positive integer spinbox
dcl_sub_(spinPint, spin)
  explicit spinPint(uint digitWidth);

signals:
  void valChg(l::pint);   ///< the value has changed
private:
  Q_OBJECT
dcl_end

//------------------------------------------------------------------------------

/// real spinbox
dcl_sub_(spinReal, QDoubleSpinBox)
  /// @c digitWidth: how many digits without scrolling (approximate)
  explicit spinReal(uint digitWidth, uint decimals);

  set_(decimals,   (uint)); ///< set the number of decimals
  set_(digitWidth, (uint)); ///< set the digitWidth
  set_(min, (real));        ///< set the minimum
  set_(max, (real));        ///< set the maximum
  set_(val, (real));        ///< set the value

  mth_(real,  val, ());     ///< get the value

signals:
  void valChg(real);        ///< the value has changed

private:
  Q_OBJECT
dcl_end

//------------------------------------------------------------------------------
}
// eof
