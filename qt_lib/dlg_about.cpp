// (qt_lib)

#include "dlg_about.hpp"
#include "inc/defs.inc"
#include <qt_lib/wgt_inc.hpp>
#include <QApplication>
#include <QDialogButtonBox>
#ifdef Q_OS_MAC
  #include <QToolTip>
#endif

namespace l_qt {
//------------------------------------------------------------------------------

DlgAbout::DlgAbout(QWidget *parent) : base(parent, Qt::Dialog) {}

int DlgAbout::exec() {
  construct();
  return base::exec();
}

void DlgAbout::construct() {
  int PAD = 12;

  setWindowTitle(QString("About %1").arg(qApp->applicationName()));

  auto vbLayout = new l_qt::vbox;
  setLayout(vbLayout);
  vbLayout->setSpacing(PAD);
  vbLayout->setSizeConstraint(QLayout::SetFixedSize);

  auto& hbLogoInfo = vbLayout->hb();
  hbLogoInfo.setSpacing(PAD);

  auto& vbLogo = hbLogoInfo.vb();

  auto logo = new l_qt::lbl(str::null);
  vbLogo.addWidget(logo);
  vbLogo.addStretch();
  logo->setPixmap(QPixmap(":/icon/logo"));

  auto info = new l_qt::lbl(infoText());
  info->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  info->setOpenExternalLinks(true);
#ifdef Q_OS_MAC
  // a smaller font (a hint found in Qt source code)
  info->setFont(QToolTip::font());
#endif

  hbLogoInfo.addWidget(info);

  auto hline = []() {
    auto frame = new QFrame;
    frame->setFrameShape(QFrame::HLine);
    frame->setFrameShadow(QFrame::Sunken);
    return frame;
  };

  auto ex = extra();
  if (ex) {
    vbLayout->addWidget(hline());
    vbLayout->addLayout(ex);
  }

  vbLayout->addWidget(hline());

  auto bb = new QDialogButtonBox(QDialogButtonBox::Ok);
  vbLayout->addWidget(bb);

  connect(bb, &QDialogButtonBox::accepted, this, &QDialog::accept);
}

//------------------------------------------------------------------------------
}
// eof
