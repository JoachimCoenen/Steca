// c
#include <qt/app.h>
#include <qt/win.h>

struct app : qt::app { BASE(qt::app)
  using base::base;
  int exec();
};

struct win : qt::win { BASE(qt::win)
  using base::base;
 ~win();
};

win::~win() {
}

int app::exec() {
  try {
    win w;
    w.show();

    int res = base::exec();

    return res;

  } catch (std::exception const& e) {
    qWarning("Fatal error: %s", e.what());
    return -1;
  }
}

int main(int argc, char* argv[]) {
  app a(argc, argv);
  a.exec();
}

// eof
