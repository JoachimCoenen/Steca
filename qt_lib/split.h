// (qt_lib)

#ifndef QT_LIB_SPLIT_H
#define QT_LIB_SPLIT_H

#include <dev_lib/inc/defs_h.h>
#include <QSplitter>

namespace l_qt {
//------------------------------------------------------------------------------

struct win;
struct split;
struct box;
struct panel;

dcl_sub_(split, QSplitter)
  split& hs();
  split& vs();

  split& add(QWidget*);
  panel& addPanel();

protected:
  split();
dcl_end

dcl_sub_(hSplit, split)
protected:
  hSplit();

  friend struct win;
  friend struct split;
  friend struct box;
dcl_end

dcl_sub_(vSplit, split)
protected:
  vSplit();

  friend struct win;
  friend struct split;
  friend struct box;
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof