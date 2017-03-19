

void OneDataset::collectIntens(core::Session::rc session, typ::Image const* intensCorr,
                               inten_vec& intens, uint_vec& counts,gma_rge::rc rgeGma,
                               tth_t minTth, tth_t deltaTth)

//------------------------------------------------------------------------------

Dataset::Dataset()
: datasets_(nullptr) {
}

shp_Metadata Dataset::metadata() const {
  if (md_.isNull()) {
    EXPECT (!isEmpty())
    const_cast<Cls*>(this)->md_ = shp_Metadata(new Metadata);
    Metadata *m = const_cast<Metadata*>(md_.data());

    EXPECT (!first()->metadata().isNull())
    Metadata::rc firstMd = *(first()->metadata());

    m->date         = firstMd.date;
    m->comment      = firstMd.comment;

    // sums: delta mon. count and time,
    // takes the last ones (presumed the maximum) of mon. count and time,
    // averages the rest
    for (auto& one : *this) {
       Metadata const* d = one->metadata().data();
       EXPECT (d)

       m->motorXT   += d->motorXT;
       m->motorYT   += d->motorYT;
       m->motorZT   += d->motorZT;

       m->motorOmg  += d->motorOmg;
       m->motorTth  += d->motorTth;
       m->motorPhi  += d->motorPhi;
       m->motorChi  += d->motorChi;

       m->motorPST  += d->motorPST;
       m->motorSST  += d->motorSST;
       m->motorOMGM += d->motorOMGM;

       m->nmT       += d->nmT;
       m->nmTeload  += d->nmTeload;
       m->nmTepos   += d->nmTepos;
       m->nmTeext   += d->nmTeext;
       m->nmXe      += d->nmXe;
       m->nmYe      += d->nmYe;
       m->nmZe      += d->nmZe;

       m->deltaMonitorCount += d->deltaMonitorCount;
       m->deltaTime    += d->deltaTime;

       if (m->monitorCount > d->monitorCount)
         MessageLogger::warn("decreasing monitor count in combined datasets");
       if (m->time > d->time)
         MessageLogger::warn("decreasing time in combined datasets");
       m->monitorCount = d->monitorCount;
       m->time         = d->time;
    }

    qreal fac = 1.0 / count();

    m->motorXT   *= fac;
    m->motorYT   *= fac;
    m->motorZT   *= fac;

    m->motorOmg  *= fac;
    m->motorTth  *= fac;
    m->motorPhi  *= fac;
    m->motorChi  *= fac;

    m->motorPST  *= fac;
    m->motorSST  *= fac;
    m->motorOMGM *= fac;

    m->nmT       *= fac;
    m->nmTeload  *= fac;
    m->nmTepos   *= fac;
    m->nmTeext   *= fac;
    m->nmXe      *= fac;
    m->nmYe      *= fac;
    m->nmZe      *= fac;
  }

  return md_;
}

Datasets::rc Dataset::datasets() const {
  EXPECT (datasets_)
  return *datasets_;
}

#define AVG_ONES(what)    \
  EXPECT (!isEmpty())      \
  qreal avg = 0;          \
  for (auto& one : *this) \
    avg += one->what();   \
  avg /= count();         \
  return avg;

deg Dataset::omg() const {
  AVG_ONES(omg)
}

deg Dataset::phi() const {
  AVG_ONES(phi)
}

deg Dataset::chi() const {
  AVG_ONES(chi)
}

// combined range of combined datasets
#define RGE_COMBINE(combineOp, what)  \
  EXPECT (!isEmpty())                  \
  Range rge;                          \
  for (auto& one : *this)             \
    rge.combineOp(one->what);         \
  return rge;

gma_rge Dataset::rgeGma(core::Session::rc session) const {
  RGE_COMBINE(extendBy, rgeGma(session))
}

gma_rge Dataset::rgeGmaFull(core::Session::rc session) const {
  RGE_COMBINE(extendBy, rgeGmaFull(session))
}

tth_rge Dataset::rgeTth(core::Session::rc session) const {
  RGE_COMBINE(extendBy, rgeTth(session))
}

inten_rge Dataset::rgeInten() const {
  RGE_COMBINE(intersect, rgeInten())
}

qreal Dataset::avgMonitorCount() const {
  AVG_ONES(monitorCount)
}

qreal Dataset::avgDeltaMonitorCount() const {
  AVG_ONES(deltaMonitorCount)
}

qreal Dataset::avgDeltaTime() const {
  AVG_ONES(deltaTime)
}

inten_vec Dataset::collectIntens(
    core::Session::rc session, typ::Image const* intensCorr, gma_rge::rc rgeGma) const
{
  tth_rge tthRge = rgeTth(session);
  tth_t   tthWdt = tthRge.width();

  auto cut = session.imageCut();
  uint pixWidth = session.imageSize().w - cut.left - cut.right;

  uint numBins;
  if (1 < count()) {  // combined datasets
    auto one = first();
    tth_t delta = one->rgeTth(session).width() / pixWidth;
    numBins = to_u(qCeil(tthWdt / delta));
  } else {
    numBins = pixWidth; // simply match the pixels
  }

  inten_vec intens(numBins, 0);
  uint_vec  counts(numBins, 0);

  tth_t minTth = tthRge.min, deltaTth = tthWdt / numBins;

  for (auto& one : *this)
    one->collectIntens(session, intensCorr, intens, counts, rgeGma, minTth, deltaTth);

  // sum or average
  if (session.intenScaledAvg()) {
    preal scale = session.intenScale();
    for_i (numBins) {
      auto cnt = counts.at(i);
      if (cnt > 0)
        intens[i] *= scale/cnt;
    }
  }

  return intens;
}

size2d Dataset::imageSize() const {
  EXPECT (!isEmpty())
  // all images have the same size; simply take the first one
  return first()->imageSize();
}

//------------------------------------------------------------------------------

Datasets::Datasets() {
  invalidateAvgMutables();
}

void Datasets::appendHere(shp_Dataset dataset) {
  // can be added only once
  EXPECT (!dataset->datasets_)
  dataset->datasets_ = this;

  super::append(dataset);
  invalidateAvgMutables();
}

size2d Datasets::imageSize() const {
  if (isEmpty())
    return size2d(0,0);

  // all images have the same size; simply take the first one
  return first()->imageSize();
}

qreal Datasets::avgMonitorCount() const {
  if (qIsNaN(avgMonitorCount_))
    avgMonitorCount_ = calcAvgMutable(&Dataset::avgMonitorCount);

  return avgMonitorCount_;
}

qreal Datasets::avgDeltaMonitorCount() const {
  if (qIsNaN(avgDeltaMonitorCount_))
    avgDeltaMonitorCount_ = calcAvgMutable(&Dataset::avgDeltaMonitorCount);

  return avgDeltaMonitorCount_;
}

qreal Datasets::avgDeltaTime() const {
  if (qIsNaN(avgDeltaTime_))
    avgDeltaTime_ = calcAvgMutable(&Dataset::avgDeltaTime);

  return avgDeltaTime_;
}

inten_rge::rc Datasets::rgeGma(core::Session::rc session) const {
  if (!rgeGma_.isValid())
    for (auto& dataset: *this)
      rgeGma_.extendBy(dataset->rgeGma(session));

  return rgeGma_;
}

inten_rge::rc Datasets::rgeFixedInten(core::Session::rc session, bool trans, bool cut) const {
  if (!rgeFixedInten_.isValid()) {

    TakesLongTime __;

    for (auto& dataset: *this)
      for (auto& one : *dataset) {
        if (one->image()) {
          auto& image = *one->image();
          shp_ImageLens imageLens = session.imageLens(image,*this,trans,cut);
          rgeFixedInten_.extendBy(imageLens->rgeInten(false));
        }
      }
  }

  return rgeFixedInten_;
}

Curve Datasets::avgCurve(core::Session::rc session) const {
  if (avgCurve_.isEmpty()) {
    // TODO invalidate when combinedDgram is unchecked

    TakesLongTime __;

    avgCurve_ = session.datasetLens(*combineAll(), *this, session.norm(), true, true)->makeCurve();
  }

  return avgCurve_;
}

void Datasets::invalidateAvgMutables() const {
  avgMonitorCount_ = avgDeltaMonitorCount_ = avgDeltaTime_ = NAN;
  rgeFixedInten_.invalidate();
  rgeGma_.invalidate();
  avgCurve_.clear();
}

shp_Dataset Datasets::combineAll() const {
  shp_Dataset d(new Dataset);

  for (shp_Dataset const& dataset: *this)
    for (shp_OneDataset const& one: *dataset)
      d->append(one);

  return d;
}

qreal Datasets::calcAvgMutable(qreal (Dataset::*avgMth)() const) const {
  qreal avg = 0;

  if (!isEmpty()) {
    for (auto& dataset: *this)
      avg += ((*dataset).*avgMth)();

    avg /= super::count();
  }

  return avg;
}

size2d OneDatasets::imageSize() const {
  EXPECT (!isEmpty())
  // all images have the same size; simply take the first one
  return first()->imageSize();
}

shp_Image OneDatasets::foldedImage() const {
  EXPECT (!isEmpty())
  shp_Image image(new Image(imageSize()));

  for (auto& one: *this)
    image->addIntens(*one->image_);

  return image;
}

//------------------------------------------------------------------------------
}
// eof
