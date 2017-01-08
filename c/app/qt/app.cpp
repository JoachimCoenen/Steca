// c

#include "app.h"
#include "../../cpp"

namespace c { namespace qt {
//------------------------------------------------------------------------------
app::app(int& argc, char* argv[]) : base(argc, argv) {
// TODO   setApplicationName(APPLICATION_NAME);
//    setApplicationVersion(VERSION);
//    setOrganizationName(ORGANIZATION_NAME);
//    setOrganizationDomain(ORGANIZATION_DOMAIN);

  #if defined(Q_OS_OSX)
    setStyle(QStyleFactory::create("Macintosh"));
  #elif defined(Q_OS_WIN)
    setStyle(QStyleFactory::create("Fusion"));
  #else
    setStyle(QStyleFactory::create("Fusion"));
  #endif
  TR("app" << this)
}

int app::exec() {
  return QApplication::exec();
}

//------------------------------------------------------------------------------
}}
// eof
