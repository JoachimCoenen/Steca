/** \file
 * Detector images.
 */

#ifndef CORE_REFLECTION_H
#define CORE_REFLECTION_H

#include "core_types.h"
#include "core_array2d.h"
#include "core_fit_functions.h"

namespace core {
//------------------------------------------------------------------------------

class Reflection {
public:
  enum eType {
    REFL_GAUSSIAN, REFL_LORENTZIAN, REFL_PSEUDOVOIGT1, REFL_PSEUDOVOIGT2,
  };

  static str_lst const& reflTypes();

  Reflection(eType = REFL_GAUSSIAN);

  eType getType() const;
  void  setType(eType);

  Range const& getRange() const;
  void         setRange(Range const&);

  /// a factory to make a function for fitting
  fit::PeakFunction* peakFunction() const;

private:
  eType type;
  Range range;
};

typedef QSharedPointer<Reflection> shp_Reflection;
typedef QVector<shp_Reflection>    Reflections;

//------------------------------------------------------------------------------
}

Q_DECLARE_METATYPE(core::shp_Reflection)

#endif