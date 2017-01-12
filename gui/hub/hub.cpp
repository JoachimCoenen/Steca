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

#include "hub.h"
#include <QApplication>
#include <c/cpp>

//------------------------------------------------------------------------------

Task::Task() : base(base::User), hub(nullptr), session(nullptr) {
}

Task::~Task() {
}

void Task::set(Hub& hub_, Session& session_) {
  hub = &hub_; session = &session_;
}

//------------------------------------------------------------------------------

Worker::Worker(Session& session_) : session(session_) {
}

void Worker::doWork(shTask task) {
  auto mp = task.mut_ptr();
  mp->work();
  mp->done();
}

//------------------------------------------------------------------------------

_cpp_sub_struct(Square, Task)
  Square(int i_) : i(i_) {}

  Task* clone() { return new Square(i); }

  void work() {
    i = session->long_square(i);
  }

  void done() {
    hub->done_long_square(i);
  }

  int i;
_cpp_sub_struct_end

//------------------------------------------------------------------------------

Hub::Hub() : session(), thread(), worker(session) {
  registerMetaTypes();

  worker.moveToThread(&thread);
  connect(this, &This::workToDo, &worker, &Worker::doWork);
  thread.start();

  post(new Square(8));
}

Hub::~Hub() {
  thread.quit();
  thread.wait();
}

void Hub::post(Task* event) {
  QApplication::postEvent(this, event);
}

void Hub::registerMetaTypes() {
  ONLY_ONCE
  qRegisterMetaType<shTask>("shTask");
}

bool Hub::event(QEvent* event) {
  auto task = dynamic_cast<Task*>(event);
  if (!task)
    return false;

  auto clone = task->clone();
  clone->set(*this, session);

  shTask sh(clone);
  emit workToDo(sh);

  return true;
}

void Hub::done_long_square(int i) {
  TR("i2" << i)
}

//------------------------------------------------------------------------------
// eof
