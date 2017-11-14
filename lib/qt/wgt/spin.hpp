// (qt_lib)

#pragma once
#include "../defs.hpp"
#include <lib/dev/inc/num.hpp>
#include <QSpinBox>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(spin, QSpinBox)
  explicit spin(uint digitWidth);

  set_(digitWidth, (uint));
  set_(min, (int));
  set_(max, (int));
  set_(val, (int));
  mth_(int, val, ());
dcl_end

dcl_sub_(spinInt, spin)
  spinInt(uint digitWidth);

signals:
  void valChg(int);
private:
  Q_OBJECT
dcl_end

dcl_sub_(spinUint, spin)
  explicit spinUint(uint digitWidth);

signals:
  void valChg(uint);
private:
  Q_OBJECT
dcl_end

dcl_sub_(spinPint, spin)
  explicit spinPint(uint digitWidth);

signals:
  void valChg(l::pint);
private:
  Q_OBJECT
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(spinReal, QDoubleSpinBox)
  explicit spinReal(uint digitWidth, uint decimals);

  set_(decimals,   (uint));
  set_(digitWidth, (uint));
  set_(min, (real));
  set_(max, (real));
  set_(val, (real));

  mth_(real,  val, ());

signals:
  void valChg(real);

private:
  Q_OBJECT
dcl_end

//------------------------------------------------------------------------------
}
// eof DOCS
