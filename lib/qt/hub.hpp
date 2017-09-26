// (qt_lib)

#pragma once
#include <lib/dev/defs.hpp>
#include <lib/dev/inc/ptr.hpp>
#include <QEvent>
#include <QObject>
#include <QThread>

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

  void doWork(Task::shr);

signals:
  void workDone(Task::shr);

private:
  Hub& hub;
  Q_OBJECT
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(Hub, QObject)
  Hub();
 ~Hub();

  void post(Task*);
  void workDone(Task::shr);

signals:
  void doWork(Task::shr);

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
