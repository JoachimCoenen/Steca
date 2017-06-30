// (qt_lib)

#ifndef QT_LIB_STR_INC_HPP
#define QT_LIB_STR_INC_HPP

#include <dev_lib/inc/defs.hpp>
#include <dev_lib/typ/vec.hpp>

#include <QString>
#include <QStringList>

namespace l_qt {
//------------------------------------------------------------------------------

QString toQt(strc);
str     fromQt(QString const&);

QStringList toQt(str_vec::rc);
str_vec     fromQt(QStringList const&);

//------------------------------------------------------------------------------
}
#endif
// eof
