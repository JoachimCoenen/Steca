// ************************************************************************** //
//
//  STeCa2:    StressTextureCalculator ver. 2
//
//! @file      output_dialogs.h
//! @brief     Output dialogs.
//!
//! @homepage  http://apps.jcns.fz-juelich.de/steca2
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2016
//! @authors   Scientific Computing Group at MLZ Garching
//! @authors   Rebecca Brydon, Jan Burle,  Antti Soininen
//! @authors   Based on the original STeCa by Christian Randau
//
// ************************************************************************** //

#ifndef OUTPUT_DIALOGS_H
#define OUTPUT_DIALOGS_H

#include "actions.h"
#include "calc/calc_reflection_info.h"
#include "gui_helpers.h"
#include "panels/panel.h"
#include "refhub.h"
#include "def/defs.h"

#include "typ/typ_variant.h"
#include <QMessageBox>

class QProgressBar;

namespace gui { namespace output {
//------------------------------------------------------------------------------

using eReflAttr = calc::ReflectionInfo::eReflAttr;

class Params : public QWidget, protected RefHub {
  SUPER(Params, QWidget)
public:
  Params(TheHub&);
 ~Params();

  void addStretch();

  uint currReflIndex() const;
  bool interpolate()   const;

protected:
  QBoxLayout *box_;

protected:
  panel::GridPanel *gpRefl_;
public:
  QComboBox        *cbRefl;
  QRadioButton     *rbCalc, *rbInterp;

protected:
  panel::GridPanel *gpGamma_;
public:
  QDoubleSpinBox   *stepGamma;
  QCheckBox        *cbLimitGamma;
  QDoubleSpinBox   *limitGammaMin, *limitGammaMax;

protected:
  panel::GridPanel *gpInterpolation_;
public:
  QDoubleSpinBox   *stepAlpha, *stepBeta;
  QDoubleSpinBox   *avgAlphaMax, *avgRadius, *idwRadius;
  QSpinBox         *threshold;
};

//------------------------------------------------------------------------------

class Tabs : public panel::TabsPanel {
  SUPER(Tabs, panel::TabsPanel)
public:
  Tabs(TheHub&);
};

//------------------------------------------------------------------------------

class Tab : public QWidget, protected RefHub {
  SUPER(Tab, QWidget)
public:
  Tab(TheHub&, Params&);

protected:
  QGridLayout *grid_;

  Params &params_;
};

//------------------------------------------------------------------------------

class Frame : public QFrame, protected RefHub {
  SUPER(Frame, QFrame)
public:
  Frame(TheHub&, rcstr title, Params*, QWidget*);

protected:
  QAction     *actClose_, *actCalculate_, *actInterpolate_;
  QToolButton *btnClose_, *btnCalculate_, *btnInterpolate_;

  QProgressBar *pb_;

  QBoxLayout *box_;
  Params     *params_;
  Tabs       *tabs_;

  typ::vec<calc::ReflectionInfos> calcPoints_, interpPoints_;

  class Table *table_;

  void calculate();
  void interpolate();

  virtual void displayReflection(uint reflIndex, bool interpolated);
};

//------------------------------------------------------------------------------

class Table : public TreeView, protected RefHub {
  SUPER(Table, TreeView)
public:
  Table(TheHub&, uint numDataColumns);

  void setColumns(str_lst::rc headers, typ::cmp_vec::rc);
  str_lst const headers();

  void clear();
  void addRow(typ::row_t::rc, bool sort);

  void sortData();

  uint rowCount() const;
  typ::row_t::rc row(uint) const;

private:
  scoped<class TableModel*> model_;
};

//------------------------------------------------------------------------------

class TabTable : public Tab {
  SUPER(TabTable, Tab)
public:
  TabTable(TheHub&, Params&, str_lst::rc headers, typ::cmp_vec::rc);

  Table *table() const { return table_; }

private:
  struct showcol_t {
    str        name;
    QCheckBox *cb;
  };

  typedef typ::vec<showcol_t> showcol_vec;

private:
  class ShowColsWidget : public QWidget {
    SUPER(ShowColsWidget, QWidget)
  public:
    ShowColsWidget(Table&, showcol_vec&);

  private:
    Table  &table_; showcol_vec& showCols_;

    QBoxLayout   *box_;
    QRadioButton *rbHidden_, *rbAll_, *rbNone_, *rbInten_, *rbTth_, *rbFWHM_;
  };

private:
  Table          *table_;
  ShowColsWidget *showColumnsWidget_;
  showcol_vec     showCols_;
};

//------------------------------------------------------------------------------

class TabSave : public Tab {
  SUPER(TabSave, Tab)
public:
  TabSave(TheHub&, Params&);

  void clearFilename();

  str path()        const { return path_->text();     }
  str fileName()    { return fileName_->text(); }

  Action* actSave() const { return actSave_; }

  str_lst const fileTags {".txt",".dat",".csv"};
  str_lst const fileSeparators {",", " ", ";"};

  void showMessage();
  void savedMessage(str message);
  void clearMessage();

protected:
  QLineEdit *path_, *fileName_;
  Action    *actBrowsePath_, *actSave_;
  QMessageBox    *filesSavedDialog_;
};

//------------------------------------------------------------------------------
}}
#endif // OUTPUT_DIALOGS_H
