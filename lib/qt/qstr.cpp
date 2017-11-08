// (qt_lib)

#include "defs.inc"

namespace l_qt {
//------------------------------------------------------------------------------

QString toQt(strc s) {
  return QString::fromStdString(s);
}

str fromQt(QString const& s) {
  return s.toStdString();
}

QStringList toQt(str_vec::rc sv) {
  QStringList sl;
  for (auto&& s : sv)
    sl.append(toQt(s));
  return sl;
}

str_vec fromQt(QStringList const& sl) {
  str_vec sv;
  for (auto&& s : sl)
    sv.add(fromQt(s));
  return sv;
}

//------------------------------------------------------------------------------
}
// eof
