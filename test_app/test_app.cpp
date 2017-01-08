// c
#include <app/app.h>
#include <app/win.h>

struct app : c::app { BASE_CONS(c::app)
  int exec();
};

struct win : c::win { BASE_CONS(c::win)
};

int app::exec() {
  try {
    win w;
    w.show();

    int res = c::app::exec();

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
