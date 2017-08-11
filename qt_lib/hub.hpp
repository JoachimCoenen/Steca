// (qt_lib)

#pragma once
#include <dev_lib/defs.hpp>
#include <dev_lib/inc/ptr.hpp>
#include <QEvent>
#include <QObject>
#include <QThread>

//------------------------------------------------------------------------------
// signalling annotation

#define emits

namespace l_qt {
//------------------------------------------------------------------------------

struct Hub;

dcl_sub_(Task, QEvent) SHARED
  Task();
 ~Task();

  void set(Hub& hub);
  virtual Task* clone() = 0;

  virtual void work() = 0;
  virtual void done() = 0;

  Hub* hub;
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(Worker, QObject)
  Worker(Hub&);

  void doWork(Task::sh);

signals:
  void workDone(Task::sh);

private:
  Hub& hub;
  Q_OBJECT
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(Hub, QObject)
  Hub();
 ~Hub();

  void post(Task*);
  void workDone(Task::sh);

signals:
  void doWork(Task::sh);

private:
  void registerMetaTypes();
  bool event(QEvent*);

  QThread thread;
  Worker  worker;
  Q_OBJECT
dcl_end

//------------------------------------------------------------------------------
}
// eof
