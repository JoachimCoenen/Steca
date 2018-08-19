//  ***********************************************************************************************
//
//  Steca: stress and texture calculator
//
//! @file      core/data/corrset.h
//! @brief     Defines class Corrset
//!
//! @homepage  https://github.com/scgmlz/Steca
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2016-2018
//! @authors   Scientific Computing Group at MLZ (see CITATION, MAINTAINER)
//
//  ***********************************************************************************************

#ifndef CORRSET_H
#define CORRSET_H

#include "core/raw/image.h"
#include "core/raw/rawfile.h"
#include "qcr/engine/cell.h"
#include <memory>

//! A correction dataset, consisting of one Rawfile and associated settings.

//! Note that "correction", throughout Steca, means "calibration" or "normalization".

class Corrset {
public:
    void fromJson(const JsonObj& obj);
    void clear();
    void clearIntens() { normalizer_.release(); }
    void removeFile();
    void loadFile(const QString& filePath);

    QJsonObject toJson() const;
    const Rawfile& raw() const { return *raw_; }
    bool hasFile() const { return raw_.get(); }
    QString fileName() const { return hasFile() ? raw_->fileName() : ""; }
    bool hasNANs() const { return hasNANs_; }
    const Image& image() const { return *corrImage_; }
    const Image* normalizer() const;

    QcrCell<bool> enabled {true};

private:
    void onCorr();
    void calcNormalizer() const;

    std::unique_ptr<const Rawfile> raw_; //!< owned by this
    mutable bool hasNANs_ {false};
    std::unique_ptr<Image> corrImage_;
    mutable std::unique_ptr<Image> normalizer_; // TODO replace by cache, and check when it is to be invalidated
};

#endif // CORRSET_H
