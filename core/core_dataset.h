/** \file
 * Dataset = a collection of images + metadata.
 */

#ifndef CORE_DATASET_H
#define CORE_DATASET_H

#include "core_defs.h"
#include "core_image.h"
#include <QMetaType>

namespace core {
//------------------------------------------------------------------------------

class File;

class Dataset final {
private:
  static QVector<str> const attributeTags;
public:
  enum eAttributes {
    DATE, COMMENT,
    MOTOR_X, MOTOR_Y, MOTOR_Z, MOTOR_OMG, MOTOR_TTH, MOTOR_PHI, MOTOR_CHI,
    MOTOR_PST, MOTOR_SST, MOTOR_OMGM, MON, DELTA_TIME,
    NUM_ATTRIBUTES
  };

  static rcstr getAttributeTag(int i); // TODO uint

public:
  Dataset(rcstr date, rcstr comment,
          qreal motorXT,  qreal motorYT,  qreal motorZT,
          qreal motorOmg, qreal motorTth, qreal motorPhi, qreal motorChi,
          qreal motorPST, qreal motorSST, qreal motorOMGM,
          qreal mon, qreal deltaTime,
          QSize const& size, intens_t const* intensities);

  qreal getNumericalAttributeValue(eAttributes const tag) const;
  str getAttributeStrValue(int) const; // TODO uint
  qreal tthMitte() const { return motorTth; } // RENAME

  File  const& getFile()  const;
  Image const& getImage() const { return image; }

  Range getRgeIntens(bool global) const;

  /// used for correction files
  void addIntensities(Dataset const&);

private:
  friend class File;
  File const *file; ///< the parent file

  str
    date, comment;

  qreal
    motorXT,  motorYT,  motorZT,  motorOmg, motorTth,
    motorPhi, motorChi, motorPST, motorSST, motorOMGM;

  qreal
    mon, deltaTime;

  Image image;
};

typedef QSharedPointer<Dataset> shp_Dataset;

//------------------------------------------------------------------------------
}

Q_DECLARE_METATYPE(core::shp_Dataset)

#endif
