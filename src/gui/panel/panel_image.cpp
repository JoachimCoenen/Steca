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

//------------------------------------------------------------------------------

dcl_sub2_(ImageWidget, RefHub, QWidget)
  ImageWidget(Hub&);

  void setPixmap(QPixmap const&);
  void setScale();

protected:
  void resizeEvent(QResizeEvent*);

private:
  core::data::CombinedSet::shp set;
  qreal scale; bool stepScale = false, showOver = false;
  QPixmap original, scaled;
  void paintEvent(QPaintEvent*);
dcl_end

ImageWidget::ImageWidget(Hub& hub) : RefHub(hub), scale(0) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  auto&& a = hub.acts;
  a.get(a.IMG_SHOW_OVER).onToggle([this](bool on) {
    showOver = on;
    update();
  });

  a.get(a.IMG_STEP_ZOOM).onToggle([this](bool on) {
    stepScale = on;
    setScale();
  });

  hub.onSigCombinedSet([this](core::data::CombinedSet::shp sh) {
    if (set != sh) {
      set = sh;
    }
  });

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

  if (stepScale && scale > 0)
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
//  // paint centered
//  auto&& margin = (size() - scaled.size()) / 2;
//  QRect rect(QPoint(margin.width(), margin.height()), scaled.size());

//  QPainter p(this);

//  // image
//  p.drawPixmap(rect.left(), rect.top(), scaled);

//  // overlay
//  if (showOver) {
//    p.setPen(Qt::lightGray);

//    // cut
//    auto cut = hub.imageCut;
//    QRect r = rect.adjusted(-1, -1, 0, 0)
//                  .adjusted(qRound(scale_ * cut.left),   qRound(scale_ * cut.top),
//                           -qRound(scale_ * cut.right), -qRound(scale_ * cut.bottom));
//    p.drawRect(r);

//    QPoint rc; rc = r.center();
//    int rcx = rc.x(), rcy = rc.y();

//    int rl, rt, rr, rb;
//    r.getCoords(&rl, &rt, &rr, &rb);
//    int rw = rr - rl;

//    // cross
//    auto off = hub_.geometry().midPixOffset;
//    auto x = qRound(rcx + scale_ * off.i);
//    auto y = qRound(rcy + scale_ * off.j);
//    p.drawLine(x, rt, x, rb);
//    p.drawLine(rl, y, rr, y);

//    // text annotations
//    auto paintText = [this, &p](QPoint pos, rcstr s, bool alignLeft) {
//      auto fm = fontMetrics();
//      if (alignLeft) pos.rx() -= fm.width(s);
//      p.drawText(pos, s);
//    };

//    p.setPen(Qt::cyan);
//    paintText(QPoint(rr - rw/5, rcy), "γ=0", false);
//  }

//  // frame
//  p.setPen(Qt::black);
//  p.drawRect(rect.adjusted(-1, -1, 0, 0));
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

    auto&& hb = tab->vb.hb();
//  box.setAlignment(hb, Qt::AlignTop);

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

//    box.addWidget((dataImageWidget_ = new ImageWidget(hub_)));

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
    auto&& tab = new Panel;
    tabs->addTab(tabImage = tab, "Correction");

    //    onSigCorrFile([&tab](data::shp_File file) {
    //      tab.setEnabled(!file.isNull());
    //    });

    //    auto&& box = tab.box();

    //    auto hb = hbox();
    //    box.addLayout(hb);
    //    box.setAlignment(hb, Qt::AlignTop);

    //    hb.add(iconButton(actions.fixedIntenImage));
    //    hb.add(iconButton(actions.stepScale));
    //    hb.add(iconButton(actions.showOverlay));
    //    hb->addStretch(1);

    //    box.addWidget((corrImageWidget_ = new ImageWidget(hub_)));
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

//  render();
}

//------------------------------------------------------------------------------
}
// eof
