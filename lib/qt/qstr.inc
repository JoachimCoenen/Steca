// (lib/qt)

/**@file
 * str <-> QString conversion
*/

#ifdef QT_LIB_STR_INC
  #error inlude only once
#endif
#define QT_LIB_STR_INC

#include <QString>
#include <QStringList>

struct str;
struct str_vec;

namespace l_qt {
//------------------------------------------------------------------------------

QString     toQt(str const& s);             ///< convert to QString from str
str         fromQt(QString const& s);       ///< convert to str from QString
QStringList toQt(str_vec const& sv);        ///< convert to QStringList from str_vec
str_vec     fromQt(QStringList const& sl);  ///< convert to str_vec from QStringList

//------------------------------------------------------------------------------
}
// eof
