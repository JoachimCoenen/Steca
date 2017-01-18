/*******************************************************************************
 * STeCa2 - StressTextureCalculator ver. 2
 *
 * Copyright (C) 2016-7 Forschungszentrum Jülich GmbH
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the COPYING and AUTHORS files for more details.
 ******************************************************************************/

#ifndef CORE_ARR_H
#define CORE_ARR_H

#include <c/c/h_data>

#define DATA_NS   core
#define DATA_NAME sz2

_c_data
  _atr(sz_t, i)
  _atr(sz_t, j)
  _c_con(())
  _c_con((sz_t, sz_t))
_c_data_end

_cpp_struct WITH_C_BASE_CONS COMPARABLE EQ_NE
  _mth(bool, empty, ())
  _mth(sz_t, size, ())
  _mth(sz2,  transposed, ())
_cpp_struct_end

_ns_dcl(sz2 operator-(sz2::rc, sz2::rc);)

#undef DATA_NAME

#define DATA_NAME arr2base

_struct
  _atr(sz2, sz)
protected:
  _ptr(void, ts)
  _con((sz2, sz_t))
  _des
  _mth(void const*, colAt, (sz_t))
_struct_end

#undef DATA_NAME

#define DATA_NAME arr2

_struct_sub_retempl(arr2base)
  _cpp_code(typedef T const& Trc;)
  _con_fwd((sz2 sz), (sz, sizeof(T)))

  _mth_inline(bool, empty, (), return sz.empty();)

  _mth_inline(Trc, at, (sz_t i, sz_t j), {
    EXPECT(i < sz.i && j < sz.j)
    return static_cast<T const*>(base::colAt(i))[j];
  })

  _mth_mut_inline(void, setAt, (sz_t i, sz_t j, Trc t), {
    EXPECT(i < sz.i && j < sz.j)
    mut(static_cast<T const*>(base::colAt(i)))[j] = t;
  })

  _mth_mut_inline(void, fill, (Trc val), {
    for (sz_t i=0; i<sz.i; ++i)
    for (sz_t j=0; j<sz.j; ++j)
      setAt(i, j, val);
  })
_struct_end

#undef DATA_NAME
#undef DATA_NS

#endif
// eof
