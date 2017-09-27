/*******************************************************************************
 * STeCa2 - StressTextureCalculator ver. 2
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

#include "panel_image.hpp"
#include <lib/qt/inc/defs.inc>
#include <lib/qt/wgt_inc.hpp>
#include "../thehub.hpp"
#include <QPainter>

namespace gui {
//------------------------------------------------------------------------------

dcl_sub_(ImageWidget, QWidget)
  ImageWidget(PanelImage::rc);

  void setPixmap(QPixmap const&);
  void setScale();

protected:
  void resizeEvent(QResizeEvent*);

private:
  PanelImage::rc panel;
  QPixmap original, scaled; qreal scale = 0;
  void paintEvent(QPaintEvent*);
dcl_end

ImageWidget::ImageWidget(PanelImage::rc panel_) : panel(panel_) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void ImageWidget::setPixmap(QPixmap const& pixmap) {
  original = pixmap;
  setScale();
}

void ImageWidget::setScale() {
  if (original.isNull()) {
    scale = 0;
  } else {
    auto sz = size();
    auto os = original.size();
    scale   = qMin(qreal(sz.width() - 2) / os.width(), qreal(sz.height() - 2) / os.height());
  }

  if (panel.imageStepScale && scale > 0)
    scale = (scale >= 1) ? l::floor(scale) : 1. / l::ceil(1. / scale);

  if (original.isNull() || !(scale > 0))
    scaled = QPixmap();
  else
    scaled = original.scaled(original.size() * scale);

  update();
}

void ImageWidget::resizeEvent(QResizeEvent* e) {
  base::resizeEvent(e);
  setScale();
}

void ImageWidget::paintEvent(QPaintEvent*) {
  // paint centered
  auto&& margin = (size() - scaled.size()) / 2;
  QRect rect(QPoint(margin.width(), margin.height()), scaled.size());

  QPainter p(this);

  // image
  p.drawPixmap(rect.left(), rect.top(), scaled);

  // overlay
  if (panel.imageShowOver) {
    p.setPen(Qt::lightGray);

    // cut
    auto cut = panel.fp().geometry.imageCut;
    QRect r = rect.adjusted(-1, -1, 0, 0)
                  .adjusted(qRound(scale * cut.left),   qRound(scale * cut.top),
                           -qRound(scale * cut.right), -qRound(scale * cut.bottom));
    p.drawRect(r);

    QPoint rc; rc = r.center();
    int rcx = rc.x(), rcy = rc.y();

    int rl, rt, rr, rb;
    r.getCoords(&rl, &rt, &rr, &rb);
    int rw = rr - rl;

    // cross
    auto off = panel.fp().geometry.midPixOffset;
    auto x = qRound(rcx + scale * off.i);
    auto y = qRound(rcy + scale * off.j);
    p.drawLine(x, rt, x, rb);
    p.drawLine(rl, y, rr, y);

    // text annotations
    auto paintText = [this, &p](QPoint pos, strc s, bool alignLeft) {
      auto fm = fontMetrics();
      if (alignLeft) pos.rx() -= fm.width(l_qt::toQt(s));
      p.drawText(pos, l_qt::toQt(s));
    };

    p.setPen(Qt::cyan);
    paintText(QPoint(rr - rw/5, rcy), "γ=0", false);
  }

  // frame
  p.setPen(Qt::black);
  p.drawRect(rect.adjusted(-1, -1, 0, 0));
}

//------------------------------------------------------------------------------

l_qt::spinPint *spinN; // TODO
l_qt::spinUint *numSlices; // TODO
l_qt::spinPint *sliceNo; // TODO
l_qt::spinPint *binNo; // TODO
l_qt::spinReal *minGamma; // TODO
l_qt::spinReal *maxGamma; // TODO

PanelImage::PanelImage(Hub& hub) : base("") {
  auto tabs = new l_qt::tabs;
  vb.add(tabs);

  {
    auto&& tab = new Panel;
    tabs->addTab(tabImage = tab, "Image");

    auto&& vb = tab->vb;
    auto&& hb = vb.hb();
    vb.align(hb, Qt::AlignTop);

    hb.add(new l_qt::actbtn(hub.acts.get(hub.acts.IMG_FIX_INTEN)));
    hb.add(new l_qt::actbtn(hub.acts.get(hub.acts.IMG_STEP_ZOOM)));
    hb.add(new l_qt::actbtn(hub.acts.get(hub.acts.IMG_SHOW_OVER)));

    hb.add(spinN = new l_qt::spinPint);
    hb.addStretch();

    hb.add(new l_qt::actbtn(hub.acts.get(hub.acts.IMG_SHOW_GAMMA)));
    hb.add(new l_qt::lbl("γ count"));
    hb.add(numSlices = new l_qt::spinUint);
    hb.add(new l_qt::lbl("#"));
    hb.add(sliceNo = new l_qt::spinPint);

    hb.add(new l_qt::lbl("min"));
    hb.add(minGamma = new l_qt::spinReal);
    hb.add(new l_qt::lbl("max"));
    hb.add(maxGamma = new l_qt::spinReal);

    minGamma->setReadOnly(true);
    maxGamma->setReadOnly(true);

    hb.add(new l_qt::lbl("bin#"));
    hb.add(binNo = new l_qt::spinPint);

    vb.addWidget(wgtImage = new ImageWidget(*this));

//    connect(spinN_, slot(QSpinBox,valueChanged,int), [this]() {
//      render();
//    });

//    connect(numSlices_, slot(QSpinBox,valueChanged,int), [this]() {
//      render();
//    });

//    connect(numSlice_, slot(QSpinBox,valueChanged,int), [this]() {
//      render();
//    });

//    connect(numBin_, slot(QSpinBox,valueChanged,int), [this]() {
//      render();
//    });
  }

  {
    auto tab = new Panel;
    tabs->addTab(tabImage = tab, "Correction");

    hub.onSigCorrFileName([this, tab](str name) {
      tab->setEnabled(!name.isEmpty());
    });

    auto&& vb = tab->vb;
    auto&& hb = vb.hb();
    vb.align(hb, Qt::AlignTop);

    hb.add(new l_qt::actbtn(hub.acts.get(hub.acts.IMG_FIX_INTEN)));
    hb.add(new l_qt::actbtn(hub.acts.get(hub.acts.IMG_STEP_ZOOM)));
    hb.add(new l_qt::actbtn(hub.acts.get(hub.acts.IMG_SHOW_OVER)));
    hb.addStretch();

    vb.add(wgtCorrection = new ImageWidget(*this));
  }

//  connect(actions.enableCorr, &QAction::toggled, [this](bool) {
//    render();
//  });

//  connect(actions.showBins, &QAction::toggled, [this]() {
//    render();
//  });

//  onSigDisplayChanged([this]() {
//    render();
//  });

//  onSigGeometryChanged([this]() {
//    render();
//  });

//  onSigNormChanged([this]() {
//    render();
//  });

//  onSigDatasetSelected([this](data::shp_Dataset dataset) {
//    setDataset(dataset);
//  });

  hub.onSigSetsInfo([this](Hub::SetsInfo info) {
    if (set != info.set || fp != info.fp) {
      sets = info.sets; set = info.set; fp = info.fp;
      renderTabs(); // TODO only one tab
    }
  });

  auto&& a = hub.acts;
  a.get(a.IMG_SHOW_OVER).onToggle([this](bool on) {
    imageShowOver = on;
    wgtImage->update();
    wgtCorrection->update();
  });

  a.get(a.IMG_STEP_ZOOM).onToggle([this](bool on) {
    imageStepScale = on;
    wgtImage->update();
    wgtCorrection->update();
  });

  renderTabs();
}

void PanelImage::renderTabs() {
  using inten_t = core::inten_t;
  auto intenImage = [](inten_t inten, inten_t maxInten, bool curved) {
    if (qIsNaN(inten))
      return qRgb(0x00, 0xff, 0xff);
    if (qIsInf(inten))
      return qRgb(0xff, 0xff, 0xff);

    if (qIsNaN(maxInten) || maxInten <= 0)
      return qRgb(0x00, 0x00, 0x00);

    inten /= maxInten;

    if (curved && inten > 0)
      inten = l::pow(inten, .6f);

    inten_t const low(.25f), mid(.5f), high(.75f);
    if (inten < low)
      return qRgb(int(0xff * inten * 4), 0, 0);
    if (inten < mid)
      return qRgb(0xff, int(0xff * (inten - low) * 4), 0);
    if (inten < high)
      return qRgb(int(0xff - (0xff * (inten - mid) * 4)), 0xff,
                  int(0xff * (inten - mid) * 4));
    return qRgb(int(0xff * (inten - high) * 4), 0xff, 0xff);
  };

  auto makeImage = [this, intenImage](core::Image::shp image, bool curvedScale) {
    QImage im;
    if (!image)
      return im;

    auto&& lens = fp().imageLens(*image, sets(), true, false);
    auto&& size = lens().size();
    if (size.isEmpty())
      return im;

    im = QImage(QSize(l::to_i(size.i), l::to_i(size.j)), QImage::Format_RGB32);

    auto&& rgeInten = lens().rgeInten(true/*hub_.isFixedIntenImageScale()*/);
    inten_t maxInten = inten_t(rgeInten.max);

    for_ij_(size.i, size.j)
      im.setPixel(l::to_i(i), l::to_i(j),
                  intenImage(lens().imageInten(i, j), maxInten, curvedScale));
    return im;
  };

  auto makeBlankPixmap = [this]() {
    auto&& size = fp().geometry.imageSize;

    QPixmap pixmap(l::to_i(size.i), l::to_i(size.j));
    pixmap.fill(QColor(0, 0, 0, 0));

    return pixmap;
  };

  auto makePixmap = [this, makeImage](core::Image::shp image) {
    return QPixmap::fromImage(makeImage(image, true/*!hub_.isFixedIntenImageScale()*/));
  };

  {
    QPixmap pixMap;

//    uint nSlices  = to_u(numSlices_->value());
//    numSlice_->setMaximum(qMax(1, to_i(nSlices)));
//    numSlice_->setEnabled(nSlices > 0);

    if (set) {
      pixMap = makePixmap(set->first()().image);
//      // 1 - based
//      uint by = qBound(1u, uint(hub_.datasetsGroupedBy()), dataset_->count());
//      uint n  = qBound(1u, to_u(spinN_->value()), by);

//      spinN_->setValue(to_i(n));
//      spinN_->setEnabled(by > 1);

//      lens_ = hub_.datasetLens(*dataset_);

//      typ::Range rge;
//      if (nSlices > 0) {
//        uint nSlice  = qMax(1u, to_u(numSlice_->value()));
//        uint iSlice  = nSlice - 1;

//        auto rgeGma = lens_->rgeGma();
//        auto min    = rgeGma.min;
//        auto wn = rgeGma.width() / nSlices;

//        rge = gma_rge(min + iSlice * wn, min + (iSlice+1) * wn);

//        minGamma_->setValue(rge.min);
//        maxGamma_->setValue(rge.max);
//      } else {
//        rge = typ::Range::infinite();;
//        minGamma_->clear();
//        maxGamma_->clear();
//      }

//      hub_.setGammaRange(rge);

//      auto oneDataset = dataset_->at(n-1);

//      numBin_->setEnabled(true);
//      if (hub_.actions.showBins->isChecked()) {
//        typ::Range rgeTth = lens_->rgeTth();
//        auto curve = lens_->makeCurve(false); // TODO factor out lens::binCount()
//        int count  = to_i(curve.count());
//        numBin_->setMaximum(count-1);
//        auto min = rgeTth.min, wdt = rgeTth.width();
//        qreal num = qreal(numBin_->value());
//        pixMap = makePixmap(*oneDataset, rge, typ::Range(min + wdt * (num/count), min + wdt * ((num+1)/count)));
//      } else {
//        pixMap = makePixmap(oneDataset->image());
//      }
    } else {
//      spinN_->setEnabled(false);
//      numBin_->setMaximum(0);
//      numBin_->setEnabled(false);

      pixMap = makeBlankPixmap();
    }

    wgtImage->setPixmap(pixMap);
  }
  {
    QPixmap pixMap = makePixmap(fp().corrImage);
    wgtCorrection->setPixmap(pixMap);
  }
}

//------------------------------------------------------------------------------
}
// eof
