/*******************************************************************************
 * REVIEW: STeCa2 - StressTextureCalculator ver. 2
 *
 * Copyright (C) 2016 Forschungszentrum Jülich GmbH 2016
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

#ifndef DOCK_METADATA_H
#define DOCK_METADATA_H

#include "panel.h"

namespace gui { namespace panel {
//------------------------------------------------------------------------------

class DockMetadata : public DockWidget, protected RefHub {
  CLASS(DockMetadata) SUPER(DockWidget)
public:
  DockMetadata(TheHub&);

private:
  class MetadataView *metadataView_;
};

//------------------------------------------------------------------------------
}}
#endif // DOCK_METADATA_H
