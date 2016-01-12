#ifndef APP_H
#define APP_H

#include "defs.h"
#include <QApplication>

class MainWin;

class App: public QApplication {
  SUPER(App,QApplication)
public:
  App(int& argc, char* argv[]);

  int exec();

private:
  MainWin *mainWin;

  bool notify(QObject*, QEvent*);
};

extern App *app;

#endif