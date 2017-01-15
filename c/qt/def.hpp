// c

#ifndef C_QT_DEF_H
#define C_QT_DEF_H

#include <c/c/lib/str.h>
#include <c/cpp/exc.hpp>
#include <QString>

typedef QString     qstr;
typedef qstr const& qstrc;

namespace c { namespace qt {
//------------------------------------------------------------------------------

[[noreturn]] void err(qstrc msg) will_exc;

//------------------------------------------------------------------------------
}}

#endif
// eof
