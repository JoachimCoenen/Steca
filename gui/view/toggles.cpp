//  ***********************************************************************************************
//
//  Steca: stress and texture calculator
//
//! @file      gui/view/toggles.cpp
//! @brief     Implements class Toggles
//!
//! @homepage  https://github.com/scgmlz/Steca
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2016-2018
//! @authors   Scientific Computing Group at MLZ (see CITATION, MAINTAINER)
//
//  ***********************************************************************************************

#include "gui/view/toggles.h"
#include "core/session.h"
#include <QObject>

Toggles::Toggles()
    : combinedDfgram {"dfg.all", &gSession->params.showAvgeDfgram,
        "All measurements", ":/icon/all"}
    , enableCorr {"enableCorr", &gSession->corrset.enabled,
        "Enable correction file"}
    , linkCuts {"linkCuts", &gSession->params.imageCut.linked,
              "Link the four cut settings", ":/icon/link"}
{
    QObject::connect(&enableCorr, &QAction::changed, [&](){ enableCorr.setText(enableCorr.isChecked() ? "On" : "Off"); });



}
