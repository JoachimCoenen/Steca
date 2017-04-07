/*******************************************************************************
 * STeCa2 - StressTextureCalculator ver. 2
 *
 * Copyright (C) 2016-7 Forschungszentrum Jülich GmbH
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the COPYING and AUTHORS files for more details.
 ******************************************************************************/

#include "hub.hpp"
#include <c2/inc/c_cpp>
#include <QApplication>

namespace gui {
//------------------------------------------------------------------------------

Task::Task() : base(base::User), hub(nullptr) {
}

Task::~Task() {
}

void Task::set(Hub& hub_) {
  hub = &hub_;;
}

//------------------------------------------------------------------------------

Worker::Worker(Hub& hub_) : hub(hub_) {}

void Worker::doWork(Task::sh task) {
  auto mp = task.ptr();
  mutp(mp)->work();
  emit workDone(task);
}

//------------------------------------------------------------------------------

Hub::Hub(Win& w) : acts(w), thread(), worker(*this) {
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

void Hub::workDone(Task::sh task) {
  mutp(task.ptr())->done();
}

void Hub::registerMetaTypes() {
  ONLY_ONCE
  qRegisterMetaType<Task::sh>("Task::sh");
}

bool Hub::event(QEvent* e) {
  auto task = dynamic_cast<Task*>(e);
  if (!task)
    return QObject::event(e);

  Task::sh sh(task->clone());
  mutp(sh.ptr())->set(*this);

  emit doWork(sh);
  return true;
}

//------------------------------------------------------------------------------
}
// eof
