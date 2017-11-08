// (qt_lib)

#ifdef QT_LIB_STR_INC
  #error inlude only once
#endif
#define QT_LIB_STR_INC

#include <QString>
#include <QStringList>

namespace l_qt {
//------------------------------------------------------------------------------

QString     toQt(strc s);
str         fromQt(QString const& s);
QStringList toQt(str_vec::rc sv);
str_vec     fromQt(QStringList const& sl);

//------------------------------------------------------------------------------
}
// eof
