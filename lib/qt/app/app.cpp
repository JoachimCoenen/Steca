// (qt_lib)

#include "app.hpp"
#include "../defs.inc"
#include <lib/dev/io/log.hpp>
#include "../dlg/dlg_msg.hpp"
#include "../win/win.hpp"
#include <QStatusBar>

namespace l_qt {
//------------------------------------------------------------------------------

app::app(int& argc, char* argv[]) : base(argc, argv) {
  setStyle("Fusion");
}

static QtMessageHandler oldHandler;
static QAtomicInt       noWarning;

app::NoWarnings::NoWarnings() {
  ++noWarning;
}

app::NoWarnings::~NoWarnings() {
  --noWarning;
}

static void messageHandler(QtMsgType type, QMessageLogContext const& ctx,
                           QString const& msg) {
  switch (type) {
  case QtDebugMsg:
    std::cerr << "Trace: " << fromQt(msg) << "\t[" << ctx.function << ']' << std::endl;
    break;
  case QtWarningMsg:
    if (0 == noWarning.load())
      dlgWarn(QApplication::activeWindow(), fromQt(msg));
    break;
  default:
    oldHandler(type, ctx, msg);
    break;
  }
}

static void waiting(bool on) {
  if (on)
    QApplication::setOverrideCursor(Qt::WaitCursor);
  else
    QApplication::restoreOverrideCursor();
}

static win *mainWin;

static void logMessage(strc msg, l_io::log::eType type) {
  EXPECT_(mainWin)

  switch (type) {
  case l_io::log::MODAL:
    dlgInfo(mainWin, msg);
    _if_clang_([[clang::fallthrough]];)
  default:
    break;
  }

  mainWin->statusBar()->showMessage(toQt(msg), 3000);
}

int app::exec(win* w) {
  mainWin = w;

  if (mainWin) {
    mainWin->show();
    oldHandler   = qInstallMessageHandler(messageHandler);
    l_io::log::set(logMessage);
    l_io::busy::set(waiting);
  }

  int res = base::exec();

  if (mainWin) {
    l_io::busy::unset();
    l_io::log::unset();
    qInstallMessageHandler(nullptr);
  }

  mainWin = nullptr;

  return res;
}

int app::safeExec(win* w) {
  try {
    return exec(w);
  } catch (l::exc::rc e) {
    qWarning("Fatal error: %s (%s)", e.msg.c_str(), e.what());
  } catch (std::exception const& e) {
    qWarning("Fatal error: %s", e.what());
  }
  return -1;
}

// Exceptions caught here; displayed in a dialog.
bool app::notify(QObject* receiver, QEvent* event) {
  try {
    return base::notify(receiver, event);
  } catch (l::exc::rc e) {
    if (!e.silent)
      qWarning("%s", e.what());
  } catch (std::exception const& e) {
    qWarning("Error: %s", e.what());
  }

  return false;
}

//------------------------------------------------------------------------------
}
// eof