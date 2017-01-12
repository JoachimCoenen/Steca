// c

#include "app.h"
#include "log.h"
#include "win.h"
#include <cpp/exc.h>
#include <c/cpp>

#include <QMessageBox>
#include <QStatusBar>
#include <QStyleFactory>
#include <iostream>

namespace c { namespace qt {
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
    std::cerr << "Trace: " << msg.toStdString()
              << "\t[" << ctx.function << ']' << std::endl;
    break;
  case QtWarningMsg:
    if (0 == noWarning)
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

static void logMessage(QString const& msg, log::eType type) {
  EXPECT(mainWin)

  QString statusMsg;
  switch (type) {
  case log::INFO:
    statusMsg = msg;
    break;
  case log::POPUP:
    QMessageBox::information(mainWin, "", msg);
    [[clang::fallthrough]];
  case log::WARN:
    statusMsg = "** " + msg + " **";
    break;
  }

  mainWin->statusBar()->showMessage(statusMsg, 3000);
}

int app::exec(win* w) {
  mainWin = w;

  if (mainWin) {
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
  } catch (exc::rc e) {
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
}}
// eof
