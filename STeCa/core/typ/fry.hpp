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

#include <c2/c/c_def.h>
#include <c2/c/lib/ptr.h>
#include <c2/c/lib/str.h>
#include <c2/cpp/exc.hpp>
#include <c2/cpp/map.hpp>

namespace core {
//-----------------------------------------------------------------------------

template <typename T>
struct fry { _typedefs(fry)
  using str = c::str;

  struct someMaker {
    virtual ~someMaker() {}
    virtual c::own<T> make() const = 0;
  };

  template <typename TT>
  struct maker : someMaker {
    c::own<T> make() const { return c::own<T>::from(new TT); }
  };

private:
  typedef c::scoped<someMaker const> scm;
  c::map<str, scm> makers;

public:
  void add(strc key, c::give_me<someMaker const> m) {
    makers[key] = m;
  }

  c::own<T> make(strc key) const may_exc {
    someMaker const *m = makers.at(key).ptr();
    if (!m) c::err("no maker ", key);
    return m->make();
  }
};
//------------------------------------------------------------------------------
}
#endif
// eof
