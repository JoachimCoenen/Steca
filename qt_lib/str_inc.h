// (qt_lib)

#ifndef QT_LIB_STR_INC_H
#define QT_LIB_STR_INC_H

#include <dev_lib/inc/defs_h.h>
#include <dev_lib/typ/vec.h>
#include <QString>
#include <QStringList>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(str_vec, l::vec<str>)
  using base::base;
dcl_end

QString toQt(strc);
str     fromQt(QString const&);

QStringList toQt(str_vec::rc);
str_vec     fromQt(QStringList const&);

//------------------------------------------------------------------------------
}
#endif
// eof
