// (qt_lib)

#pragma once
#include <dev_lib/defs.hpp>
#include <dev_lib/inc/num.hpp>
#include <QSpinBox>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(spin, QSpinBox)
  spin();

  set_(digitWidth, (uint));
  set_(min, (int));
  set_(max, (int));
  set_(val, (int));
  mth_(int, val, ());
dcl_end

dcl_sub_(spinInt, spin)
  spinInt();

signals:
  void valChg(int);
private:
  Q_OBJECT
dcl_end

dcl_sub_(spinUint, spin)
  spinUint();

signals:
  void valChg(uint);
private:
  Q_OBJECT
dcl_end

dcl_sub_(spinPint, spin)
  spinPint();

signals:
  void valChg(l::pint);
private:
  Q_OBJECT
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(spinReal, QDoubleSpinBox)
  spinReal();

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
// eof
