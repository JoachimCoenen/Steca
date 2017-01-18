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

#ifndef CORE_FRY_H
#define CORE_FRY_H

#include <c/c/h_data>
#include <c/c/lib/ptr.h>
#include <c/c/lib/str.h>
#include <c/cpp/exc.hpp>
#include <map>

#define DATA_NS   core
#define DATA_NAME fry

_struct_templ
  using str = c::str;

  struct maker {
    c::own<T> make() { return c::own<T>::from(new T); }
  };

  typedef c::shared<maker> shMaker;

  void add(strc key, shMaker m) {
    makers[key] = m;
  }

  c::own<T> make(strc key) may_exc {
    maker *m = makers[key];
    if (!m) c::err(str::cat("no maker ", key));
    return m->make();
  }

private:
  std::map<str, shMaker> makers;

_struct_end

#undef DATA_NAME
#undef DATA_NS

#endif
// eof
