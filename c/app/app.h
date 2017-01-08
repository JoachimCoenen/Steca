// c

#ifndef C_APP_H
#define C_APP_H

#include "../def/def_h"
#if __cpp__
#include "qt/app.h"

namespace c {

struct app : qt::app { BASE_CONS(qt::app)
};

}

#endif
#endif
// eof
