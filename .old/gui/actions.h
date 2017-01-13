/*******************************************************************************
 * REVIEW: STeCa2 - StressTextureCalculator ver. 2
 *
 * Copyright (C) 2016-7 Forschungszentrum Jülich GmbH
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the COPYING and AUTHORS files for more details.
 ******************************************************************************/

#ifndef ACTIONS_H
#define ACTIONS_H

#include "refhub.h"
#include "def/defs.h"

#include <QAction>

namespace gui {
//------------------------------------------------------------------------------

class Action : public QAction {
  CLASS(Action) SUPER(QAction)
public:
  Action(qstrc text, QObject*);

  Action& text(qstrc);
  Action& tip(qstrc);
  Action& key(QKeySequence);
  Action& icon(qstrc);

  virtual Action& alt(qstrc text2);
};

class TriggerAction : public Action {
  CLASS(TriggerAction) SUPER(Action)
public:
  TriggerAction(qstrc text, QObject* = nullptr);
};

class ToggleAction : public Action {
  CLASS(ToggleAction) SUPER(Action)
public:
  ToggleAction(qstrc text, QObject* = nullptr);

  Action& alt(qstrc text2);

protected:
  str text1_, text2_;
};

//------------------------------------------------------------------------------

class Actions : protected RefHub {
  CLASS(Actions) SUPER(RefHub)
public:
  Actions(TheHub&);

  Action
    *about, *online, *checkUpdate, *quit,
    *viewStatusbar, *viewFiles, *viewDatasets, *viewDatasetInfo, *viewHelp, *viewReset,
#ifndef Q_OS_OSX  // Mac has its own
    *fullScreen,
#endif
    *loadSession, *saveSession, *clearSession,
    *addFiles, *remFile, *enableCorr, *remCorr,
    *rotateImage, *mirrorImage, *linkCuts, *showOverlay, *stepScale, *showBins,
    *fixedIntenImage, *fixedIntenDgram, *combinedDgram,
    *showAveraged,
    *selRegions, *showBackground, *clearBackground, *clearReflections,
    *addReflection, *remReflection,
    *outputPolefigures, *outputDiagrams, *outputDiffractograms;

private:
  Action& trg(Action*& action, qstrc text);
  Action& tgl(Action*& action, qstrc text);
};

//------------------------------------------------------------------------------
}
#endif // ACTIONS_H
