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

#include "panel_diffractogram.hpp"
#include <lib/qt/inc/defs.inc>
#include <lib/qt/wgt_inc.hpp>
#include <core/data/fit.hpp>
#include "../QCP/qcustomplot.h"
#include "../thehub.hpp"
#include <QCoreApplication>

namespace gui {
//------------------------------------------------------------------------------

using Fit    = core::data::Fit;
using Range  = core::Range;
using Ranges = core::Ranges;

struct Plot;

dcl_sub2_(Overlay, RefHub, QWidget)
  Overlay(Hub&, Plot&);

  void setMargins(int left, int right);

private:
  Plot &plot;
  QColor remColor, color, bgColor, reflColor;
  int marginLeft, marginRight;

protected:
  void enterEvent(QEvent*);
  void leaveEvent(QEvent*);
  void mouseMoveEvent(QMouseEvent*);
  void mousePressEvent(QMouseEvent*);
  void mouseReleaseEvent(QMouseEvent*);

  void paintEvent(QPaintEvent*);

  bool hasCursor, mouseDown;
  int  cursorPos, mouseDownPos;

  void updateCursorRegion();
dcl_end

dcl_sub2_(Plot, RefHub, QCustomPlot)
  Plot(Hub&);

  ptr_(Overlay, overlay);
  atr_(Fit::eWhat, tool);

  void  setTool(Fit::eWhat);

  //  void  plot(typ::Curve::rc, typ::Curve::rc, typ::Curve::rc,
  //             typ::curve_vec::rc, uint);

  Range fromPixels(int, int);

  void setNewReflRange(Range::rc);
  void updateBg();

  //  void clearReflLayer();

  QColor bgRgeColor, reflRgeColor;

  //  void enterZoom(bool);

  //protected:
  //  void addBgItem(typ::Range::rc);
  void resizeEvent(QResizeEvent*);

  //private:
  //  Diffractogram &diffractogram_;

  //  bool showBgFit_;

  QCPGraph *bgGraph, *dgramGraph, *dgramBgFittedGraph, *dgramBgFittedGraph2,
           *guesses, *fits;

  //  typ::vec<QCPGraph*>       reflGraph_;
dcl_end

//------------------------------------------------------------------------------

Overlay::Overlay(Hub& hub, Plot& plot_)
: RefHub(hub), base(&plot_), plot(plot_), hasCursor(false), mouseDown(false)
//, cursorPos_(0), mouseDownPos_(0)
{
  setMouseTracking(true);
  setMargins(0, 0);

  remColor  = QColor(0xf8, 0xf8, 0xff, 0x90);
  bgColor   = QColor(0x98, 0xfb, 0x98, 0x70);
  reflColor = QColor(0x87, 0xce, 0xfa, 0x70);
}

void Overlay::setMargins(int left, int right) {
  marginLeft  = left;
  marginRight = right;
}

void Overlay::enterEvent(QEvent*) {
  hasCursor = true;
  updateCursorRegion();
}

void Overlay::leaveEvent(QEvent*) {
  hasCursor = false;
  updateCursorRegion();
}

void Overlay::mouseMoveEvent(QMouseEvent* e) {
  updateCursorRegion();
  cursorPos = qBound(marginLeft, e->x(), width() - marginRight);
  updateCursorRegion();
  if (mouseDown)
    update();
  qApp->processEvents();
}

void Overlay::mousePressEvent(QMouseEvent* e) {
  mouseDownPos = cursorPos;
  mouseDown    = true;
  auto&& addColor = Fit::eWhat::BACKGROUND == plot.tool ? bgColor: reflColor;
  color = Qt::LeftButton == e->button() ? addColor : remColor;
  update();
}

void Overlay::mouseReleaseEvent(QMouseEvent* e) {
  mouseDown = false;

  Range range(plot.fromPixels(mouseDownPos, cursorPos));
  switch (plot.tool) {
  case Fit::eWhat::NONE:
    break;
  case Fit::eWhat::BACKGROUND:
    if (Qt::LeftButton == e->button())
      hub.addBg(range);
    else
      hub.remBg(range);
    break;
  case Fit::eWhat::PEAK:
    plot.setNewReflRange(range);
    break;
  }

  update();
}

void Overlay::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  QRect g = geometry();

  if (mouseDown) {
    g.setLeft(qMin(mouseDownPos, cursorPos));
    g.setRight(qMax(mouseDownPos, cursorPos));
    painter.fillRect(g, color);
  }

  if (hasCursor) {
    QLineF cursor(cursorPos, g.top(), cursorPos, g.bottom());
    painter.setPen(Qt::red);
    painter.drawLine(cursor);
  }
}

void Overlay::updateCursorRegion() {
  auto g = geometry();
  // updating 2 pixels would to work both on Linux & Mac, but we'll do 8* for less flicker
  update(cursorPos - 8*1, g.top(), 8*2, g.height());
}

//------------------------------------------------------------------------------

//DiffractogramPlot::DiffractogramPlot(TheHub& hub, Diffractogram& diffractogram)
//: RefHub(hub), diffractogram_(diffractogram), showBgFit_(false)
Plot::Plot(Hub& hub) : RefHub(hub)
, overlay(new Overlay(hub, *this)), tool(Fit::eWhat::NONE) {

  auto&& ar = axisRect();

  // fix margins
  QFontMetrics fontMetrics(font());
  int em = fontMetrics.width('M'), ascent = fontMetrics.ascent();

  QMargins margins(6 * em, ascent, em, 2 * ascent);
  ar->setAutoMargins(QCP::msNone);
  ar->setMargins(margins);

  mut(*overlay).setMargins(margins.left(), margins.right());

  bgRgeColor   = QColor(0x98, 0xfb, 0x98, 0x50);
  reflRgeColor = QColor(0x87, 0xce, 0xfa, 0x50);

  // colours
  setBackground(palette().background().color());
  ar->setBackground(Qt::white);

  // graphs in the "main" layer; in the display order
  bgGraph            = addGraph();
  dgramGraph         = addGraph();
  dgramBgFittedGraph = addGraph();
  (dgramBgFittedGraph2 = addGraph())->setVisible(false);

  bgGraph->setPen(QPen(QColor(0x21, 0xa1, 0x21, 0xff), 2));

  dgramBgFittedGraph->setPen(QPen(Qt::black, 2));

  dgramBgFittedGraph2->setLineStyle(QCPGraph::LineStyle::lsNone);
  dgramBgFittedGraph2->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssDisc,QColor(255,0,0),4));

  dgramGraph->setLineStyle(QCPGraph::LineStyle::lsNone);
  dgramGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssDisc,Qt::gray,2));

  // background regions
  addLayer("bg", layer("background"), QCustomPlot::limAbove);
  // reflections
  addLayer("refl", layer("main"), QCustomPlot::limAbove);
  // reflections
  addLayer("marks", layer("refl"), QCustomPlot::limAbove);

  setCurrentLayer("marks");

  guesses = addGraph();
  guesses->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 8));
  guesses->setLineStyle(QCPGraph::lsNone);
  guesses->setPen(QPen(Qt::darkGray));

  fits = addGraph();
  fits->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 8));
  fits->setLineStyle(QCPGraph::lsNone);
  fits->setPen(QPen(Qt::red));

//  onSigReflectionData([this](calc::shp_Reflection reflection) {
//    guesses_->clearData();
//    fits_->clearData();

//    if (reflection && diffractogram_.dataset()) {
//      auto&& fun = reflection->peakFunction();

//      auto gp = fun.guessedPeak();
//      if (gp.isValid()) {
//        guesses_->addData(gp.x, gp.y);
//        auto gw2 = fun.guessedFWHM() / 2;
//        guesses_->addData(gp.x - gw2, gp.y / 2);
//        guesses_->addData(gp.x + gw2, gp.y / 2);
//      }

//      auto fp = fun.fittedPeak();
//      if (fp.isValid()) {
//        fits_->addData(fp.x, fp.y);
//        auto fw2 = fun.fittedFWHM() / 2;
//        fits_->addData(fp.x - fw2, fp.y / 2);
//        fits_->addData(fp.x + fw2, fp.y / 2);
//      }
//    }
//  });

//  connect(hub_.actions.showBackground, &QAction::toggled, [this](bool on) {
//    showBgFit_ = on;
//    updateBg();
//  });

//  onSigBgChanged([this]() {
//    updateBg();
//  });

  mut(tool) = Fit::eWhat::NONE;
}

void Plot::setTool(Fit::eWhat tool_) {
  mut(tool) = tool_;
  updateBg();
}

//void DiffractogramPlot::plot(typ::Curve::rc dgram, typ::Curve::rc dgramBgFitted,
//                             typ::Curve::rc bg, typ::curve_vec::rc refls,
//                             uint currReflIndex)
//{
//  if (dgram.isEmpty()) {
//    xAxis->setVisible(false);
//    yAxis->setVisible(false);

//    bgGraph_->clearData();
//    dgramGraph_->clearData();
//    dgramBgFittedGraph_->clearData();
//    dgramBgFittedGraph2_->clearData();

//    clearReflLayer();
//  } else {
//    auto tthRange = dgram.rgeX();

//    typ::Range intenRange;
//    if (hub_.isFixedIntenDgramScale()) {
//      ENSURE(!diffractogram_.dataset().isNull())
//      auto lens = hub_.datasetLens(*diffractogram_.dataset());
//      intenRange = lens->rgeInten();
//    } else {
//      intenRange = dgramBgFitted.rgeY();
//      intenRange.extendBy(dgram.rgeY());
//    }

//    xAxis->setRange(tthRange.min, tthRange.max);
//    yAxis->setRange(qMin(0., intenRange.min), intenRange.max);
//    yAxis->setNumberFormat("g");
//    xAxis->setVisible(true);
//    yAxis->setVisible(true);

//    if (showBgFit_) {
//      bgGraph_->setData(bg.xs().sup(), bg.ys().sup());
//    } else {
//      bgGraph_->clearData();
//    }

//    dgramGraph_->setData(dgram.xs().sup(), dgram.ys().sup());
//    dgramBgFittedGraph_->setData(dgramBgFitted.xs().sup(), dgramBgFitted.ys().sup());
//    dgramBgFittedGraph2_->setData(dgramBgFitted.xs().sup(), dgramBgFitted.ys().sup());

//    clearReflLayer();
//    setCurrentLayer("refl");

//    for_i (refls.count()) {
//      auto&& r = refls.at(i);
//      auto* graph = addGraph();
//      reflGraph_.append(graph);
//      graph->setPen(QPen(Qt::green, i == currReflIndex ? 2 : 1));
//      graph->setData(r.xs().sup(), r.ys().sup());
//    }
//  }

//  replot();
//}

Range Plot::fromPixels(int pix1, int pix2) {
  return Range::safeFrom(xAxis->pixelToCoord(pix1),
                         xAxis->pixelToCoord(pix2));
}

void Plot::setNewReflRange(Range::rc range) {
//  diffractogram_.setCurrReflNewRange(range);
//  updateBg();
}

void Plot::updateBg() {
//  clearItems();

//  switch (tool_) {
//  case eTool::BACKGROUND: {
//    typ::Ranges::rc rs = hub_.bgRanges();
//    for_i (rs.count())
//      addBgItem(rs.at(i));
//    break;
//  }
//  case eTool::PEAK_REGION:
//    addBgItem(diffractogram_.currReflRange());
//    break;
//  case eTool::NONE:
//    break;
//  }

//  diffractogram_.render();
}

//void DiffractogramPlot::clearReflLayer() {
//  for (auto g : reflGraph_)
//    removeGraph(g);
//  reflGraph_.clear();
//}

//void DiffractogramPlot::enterZoom(bool on) {
//  overlay_->setHidden(on);
//  dgramBgFittedGraph2_->setVisible(on);
//}

//void DiffractogramPlot::addBgItem(typ::Range::rc range) {
//  setCurrentLayer("bg");

//  QColor color;
//  switch (hub_.fittingTab()) {
//  case eFittingTab::BACKGROUND:
//    color = bgRgeColor_;
//    break;
//  case eFittingTab::REFLECTIONS:
//    color = reflRgeColor_;
//    break;
//  default:
//    break;
//  }

//  auto ir = new QCPItemRect(this);
//  ir->setPen(QPen(color));
//  ir->setBrush(QBrush(color));
//  auto br = ir->bottomRight;
//  br->setTypeY(QCPItemPosition::ptViewportRatio);
//  br->setCoords(range.max, 1);
//  auto tl = ir->topLeft;
//  tl->setTypeY(QCPItemPosition::ptViewportRatio);
//  tl->setCoords(range.min, 0);
//  addItem(ir);
//}

void Plot::resizeEvent(QResizeEvent* e) {
  base::resizeEvent(e);
  auto size = e->size();
  mut(*overlay).setGeometry(0, 0, size.width(), size.height());
}

//------------------------------------------------------------------------------

PanelDiffractogram::PanelDiffractogram(Hub& hub) : base("") {
  auto tabs = new l_qt::tabs;
  vb.add(tabs);
  tabs->addTab((tab = new Panel()), "Diffractogram");

  tab->vb.add((plot = new Plot(hub)));

  auto&& hb = tab->vb.hb();

  hb.add(new l_qt::lbl("norm:"));
  hb.add(new l_qt::cbo(core::normStrLst));
  hb.add(new l_qt::lbl(" inten:"));
  hb.add(new l_qt::rio("sum"));
  hb.add(new l_qt::rio("avg ×"));
  hb.add(new l_qt::spinReal()); // em4_2, 0.001 setDecimals(3);

  hb.addStretch();
  hb.add(new l_qt::actbtn(hub.acts.get(hub.acts.DIFF_ZOOM)));
  hb.addStretch();

  hb.add(new l_qt::chk(hub.acts.get(hub.acts.DIFF_ALL_DSETS)));
  hb.add(new l_qt::chk(hub.acts.get(hub.acts.DIFF_FIX_INTEN)));
}

//------------------------------------------------------------------------------
}
// eof
