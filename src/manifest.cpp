#include "manifest.h"

char const
  *APPLICATION_NAME      = "STeCa2",
  *APPLICATION_VERSION   =
    #include "../VERSION"
  ,
  *ORGANIZATION_NAME     = "Scientific Computing at MLZ",
  *ORGANIZATION_DOMAIN   = "apps.jcns.fz-juelich.de",

  *STECA2_PAGES_URL      = "http://apps.jcns.fz-juelich.de/steca2",
  *STECA2_VERSION_URL    = "http://apps.jcns.fz-juelich.de/src/STeCa2/version.php",
  *STECA2_DOWNLOAD_URL   = "https://github.com/scgmlz/STeCa2/releases";

/*:>

= STeCa2 {img: img/icon.png}

This is the homepage of
{*STeCa2}: {/StressTextureCalculator version 2}.
----
{:[Download latest code & Windows executable]|https://github.com/scgmlz/STeCa2/releases}
----
.

{img: img/image_ex1.png|detector image} \
{=>} {img: img/diffractogram.png|diffractogram} \
{=>} {img: img/stier.png|pole figure}

== Overview

. Steca2 is a software tool for analysis of histogram data collected by area detectors.
It is developed primarily to support the {:STRESS-SPEC diffractometer|http://www.mlz-garching.de/stress-spec} at FRM-II.

. The main functions of Steca2 are:

* Import of histograms and associated metadata from data files.
* Variable grouping of multiple data sets.
* Correction of unequal detector pixel sensitivity.
* Calculation of diffractograms, either from complete images or from image "slices" \
  with specified range of {/γ}.
* Semi-automatic diffractogram peak fitting.
* Calculation of pole figure points and their interpolation.
* Calculation of diagrams that show dependencies between imported metadata and calculated data.
* Export of results in various data formats for further processing.

{img:.right img/labs_stier.png} {img:.right img/labs_loop.png}

. Steca2 is based on the original STeCa software developed by Christian Randau.
For further information about the original STeCa, see the
{:JAC article|https://www.researchgate.net/publication/264525178_StressTextureCalculator_A_software_tool_to_extract_texture_strain_and_microstructure_information_from_area-detector_measurements}
by C. Randau, U. Garbe and H.-G. Brokmeier, and C. Randau's
{:dissertation|https://dokumente.ub.tu-clausthal.de/receive/import_mods_00000047}.

. Steca2 has a newly designed user interface and re-implemented calculations with
increased speed of processing. The imported and computed data can be interactively
manipulated and inspected. The code has been written to allow long-term maintenance,
further development and extensions, and future support of new data formats.

. Steca2 has been developed by Rebecca Brydon, Jan Burle, and Antti Soininen within
the {:Scientific Computing Group|http://apps.jcns.fz-juelich.de}
of JCNS at MLZ Garching.


== Current version

The current version of Steca2 is 2.0.4.
It is in daily use at STRESS-SPEC.
<:*/

// eof