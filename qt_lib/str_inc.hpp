// (qt_lib)

#pragma once

#include <dev_lib/defs.hpp>
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
// eof
