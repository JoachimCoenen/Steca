// c

#ifndef C_QT_DEF_HPP
#define C_QT_DEF_HPP

#include "../c/lib/str.h"
#include "../cpp/exc.hpp"

class QString;

typedef QString     qstr;
typedef qstr const& qstrc;

c::str toStr(qstrc);

#endif
// eof
