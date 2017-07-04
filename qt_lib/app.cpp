// (qt_lib)

#include "app.hpp"
#include <dev_lib/defs.inc>
#include <dev_lib/io/log.hpp>

#include "dlg_msg.hpp"
#include "str_inc.hpp"

#include <QStatusBar>
#include <QStyleFactory>
#include <iostream>

namespace l_qt {
//------------------------------------------------------------------------------

app::app(int& argc, char* argv[]) : base(argc, argv) {
  #if defined(Q_OS_OSX)
    setStyle(QStyleFactory::create("Macintosh"));
  #elif defined(Q_OS_WIN)
    setStyle(QStyleFactory::create("Fusion"));
  #else
    setStyle(QStyleFactory::create("Fusion"));
#endif
}

static QtMessageHandler oldHandler;
static QAtomicInt       noWarning;

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
    busy_indicator::handler = waiting;
    oldHandler   = qInstallMessageHandler(messageHandler);
    l_io::log::set(logMessage);
  }

  int res = base::exec();

  if (mainWin) {
    l_io::log::unset();
    qInstallMessageHandler(nullptr);
    busy_indicator::handler = nullptr;
  }

  mainWin = nullptr;

  return res;
}

int app::safeExec(win* w) {
  try {
    return exec(w);
  } catch (std::exception const& e) {
    qWarning("Fatal error: %s", e.what());
    return -1;
  }
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

busy_indicator::busy_indicator() {
  if (handler)
    handler(true);
}

busy_indicator::~busy_indicator() {
  if (handler)
    handler(false);
}

void (*busy_indicator::handler)(bool) = nullptr;

//------------------------------------------------------------------------------
}
// eof
