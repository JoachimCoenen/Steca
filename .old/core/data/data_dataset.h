//------------------------------------------------------------------------------

class OneDatasets : public typ::vec<shp_OneDataset> {
  CLASS(OneDatasets) SUPER(typ::vec<shp_OneDataset>)
public:
  typ::size2d    imageSize()   const;
  typ::shp_Image foldedImage() const;
};

//------------------------------------------------------------------------------

// 1 or more OneDataset(s)
class Dataset final : public OneDatasets {
  CLASS(Dataset) SUPER(OneDatasets)
  friend class Datasets;
public:
  Dataset();

  shp_Metadata    metadata() const;
  Datasets const& datasets() const;

  typ::deg omg() const;
  typ::deg phi() const;
  typ::deg chi() const;

  gma_rge  rgeGma(core::Session const&) const;
  gma_rge  rgeGmaFull(core::Session const&) const;
  tth_rge  rgeTth(core::Session const&) const;

  inten_rge rgeInten() const;

  qreal    avgMonitorCount()      const;
  qreal    avgDeltaMonitorCount() const;
  qreal    avgDeltaTime()         const;

  inten_vec collectIntens(core::Session const&, typ::Image const* intensCorr,
                          gma_rge::rc) const;

_private
  // all dataset(s) must have the same image size
  typ::size2d imageSize()  const;

  Datasets*    datasets_; // here it belongs (or can be nullptr)
  shp_Metadata md_;       // on demand, compute once
};

//------------------------------------------------------------------------------

class Datasets final : public typ::vec<shp_Dataset> {
  CLASS(Datasets) SUPER(typ::vec<shp_Dataset>)
public:
  Datasets();

  void appendHere(shp_Dataset);

  typ::size2d imageSize() const;

  qreal avgMonitorCount()      const;
  qreal avgDeltaMonitorCount() const;
  qreal avgDeltaTime()         const;

  inten_rge::rc  rgeGma(core::Session const&) const;
  inten_rge::rc  rgeFixedInten(core::Session const&, bool trans, bool cut) const;

  typ::Curve avgCurve(core::Session const&) const;

  void  invalidateAvgMutables() const;

_private
  shp_Dataset combineAll() const;
  qreal calcAvgMutable(qreal (Dataset::*avgMth)() const) const;

  // computed on demand (NaNs or emptiness indicate yet unknown values)
  mutable qreal avgMonitorCount_, avgDeltaMonitorCount_, avgDeltaTime_;
  mutable inten_rge rgeFixedInten_;
  mutable gma_rge   rgeGma_;
  mutable typ::Curve avgCurve_;
};

//------------------------------------------------------------------------------
}
#endif // DATA_DATASET_H
#endif // FORWARD_DECLARATIONS
