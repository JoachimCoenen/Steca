// (app_lib)

#include "app.h"
#include "../inc/defs_cpp.h"
#include "../inc/exc.h"
#include "log.h"

#include <QMessageBox>
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
                           strc msg) {
  switch (type) {
  case QtDebugMsg:
    std::cerr << "Trace: " << msg.toStdString()
              << "\t[" << ctx.function << ']' << std::endl;
    break;
  case QtWarningMsg:
    if (0 == noWarning.load())
      QMessageBox::warning(QApplication::activeWindow(), qAppName(), msg);
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

static void logMessage(strc msg, log::eType type) {
  EXPECT_(mainWin)

  QString statusMsg;
  switch (type) {
  case log::INFO:
    statusMsg = msg;
    break;
  case log::POPUP:
    QMessageBox::information(mainWin, "", msg);
    _if_clang_([[clang::fallthrough]];)
  case log::WARN:
    statusMsg = "** " + msg + " **";
    break;
  }

  mainWin->statusBar()->showMessage(statusMsg, 3000);
}

int app::exec() {
  return execWin(nullptr);
}

int app::execWin(win& w) {
  return execWin(&w);
}

int app::runWin(win& w) {
  try {
    return execWin(w);
  } catch (std::exception const& e) {
    qWarning("Fatal error: %s", e.what());
    return -1;
  }
}

int app::execWin(win* w) {
  mainWin = w;

  if (mainWin) {
    mainWin->show();
    busy_indicator::handler = waiting;
    oldHandler   = qInstallMessageHandler(messageHandler);
    log::handler = logMessage;
  }

  int res = base::exec();

  if (mainWin) {
    log::handler = nullptr;
    qInstallMessageHandler(nullptr);
    busy_indicator::handler = nullptr;
  }

  mainWin = nullptr;

  return res;
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
