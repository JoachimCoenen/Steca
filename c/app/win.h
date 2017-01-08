// c

#ifndef C_WIN_H
#define C_WIN_H

#include "../def/def_h"
#if __cpp__
#include "qt/win.h"

namespace c {

struct win : qt::win { BASE_CONS(qt::win)
};

}

#endif
#endif
// eof
