// (qt_lib)

#include "hub.hpp"
#include "defs.inc"
#include <QApplication>

namespace l_qt {
//------------------------------------------------------------------------------

Task::Task() : base(base::User), hub(nullptr) {}

Task::~Task() {}

void Task::set(Hub& hub_) {
  hub = &hub_;
}

//------------------------------------------------------------------------------

Worker::Worker(Hub& hub_) : hub(hub_) {}

void Worker::doWork(Task::shr task) {
  mut(task()).work();
  emit workDone(task);
}

//------------------------------------------------------------------------------

Hub::Hub() : thread(), worker(*this) {
  registerMetaTypes();

  worker.moveToThread(&thread);
  connect(this,    &Self::doWork,     &worker, &Worker::doWork);
  connect(&worker, &Worker::workDone, this,    &Self::workDone);

  thread.start();
}

Hub::~Hub() {
  thread.quit();
  thread.wait();
}

void Hub::post(Task* event) {
  QApplication::postEvent(this, event);
}

void Hub::workDone(Task::shr task) {
  mut(task()).done();
}

void Hub::registerMetaTypes() {
  ONLY_ONCE
  qRegisterMetaType<Task::shr>("Task::sh");
}

bool Hub::event(QEvent* e) {
  auto task = dynamic_cast<Task*>(e);
  if (!task)
    return QObject::event(e);

  auto&& sh = l::sh(task->clone());
  mut(*sh).set(*this);

  emit doWork(sh);
  return true;
}

//------------------------------------------------------------------------------
}
// eof
