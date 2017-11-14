// (qt_lib)

#include "settings.hpp"
#include "../defs.inc"

namespace l_qt {
//------------------------------------------------------------------------------

pcstr const Settings::GROUP_MAINWIN = "config";
pcstr const Settings::GEOMETRY      = "geometry";
pcstr const Settings::STATE         = "state";

Settings::Settings(strc group) {
  setFallbacksEnabled(false);
  beginGroup(toQt(group));
}

Settings::~Settings() {
  endGroup();
}

var Settings::readVar(strc key, var::rc def) const {
  return value(toQt(key), def);
}

Settings::ref Settings::saveVar(strc key, var::rc val) {
  setValue(toQt(key), val); RTHIS
}

bool Settings::readBool(strc key, bool def) const {
  return readVar(key, var(def)).toBool();
}

Settings::ref Settings::saveBool(strc key, bool val) {
  saveVar(key, var(val)); RTHIS
}

int Settings::readInt(strc key, int def) const {
  try {
    return readVar(key, var()).toInt();
  } catch (l::exc&) {
    return def;
  }
}

Settings::ref Settings::saveInt(strc key, int val) {
  saveVar(key, var(val)); RTHIS
}

real Settings::readReal(strc key, qreal def) const {
  try {
    return readVar(key, var()).toReal();
  } catch (l::exc&) {
    return def;
  }
}

Settings::ref Settings::saveReal(strc key, qreal val) {
  saveVar(key, var(val)); RTHIS
}

str Settings::readStr(strc key, strc def) const {
  return readVar(key, var(toQt(def))).toStr();
}

Settings::ref Settings::saveStr(strc key, strc val) {
  saveVar(key, var(toQt(val))); RTHIS
}

//------------------------------------------------------------------------------
}
// eof DOCS
