// (lib/qt)

/** @file
 * A "hub": the idea is that in an application there would be a special object,
 * typically in a single instance (but there can, be, z.B. a separate instance
 * of a hub for each window. The hub is the central point of larger
 * communication among components; the hub also owns and controls the global
 * data. Gui etc. do request changes and work be done by the hub; the hub calls
 * the computational core. When the work is done, the hub emits signals. Work
 * can be delegated to workers.
 */

#pragma once

#include "../defs.hpp"
#include <lib/dev/inc/ptr.hpp>
#include <QEvent>
#include <QObject>
#include <QThread>

namespace l_qt {
//------------------------------------------------------------------------------

struct Hub;

/// elementary task
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

/// a worker
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

/// the hub (general - extend in your application)
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
