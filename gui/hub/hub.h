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

#ifndef GUI_HUB_H
#define GUI_HUB_H

#include <core/session.h>

#include <QObject>
#include <QEvent>
#include <QThread>

struct Hub;

_cpp_sub_struct(Task, QEvent) NO_COPY(Task)
  Task();
 ~Task();

  void set(Hub& hub, Session& session);
  virtual Task* clone() = 0;

  virtual void work() = 0;
  virtual void done() = 0;

  Hub* hub; Session* session;
_cpp_sub_struct_end

typedef c::shared<Task> shTask;
Q_DECLARE_METATYPE(shTask)

_cpp_sub_struct(Worker, QObject)
  Worker(Session&);

  void doWork(shTask);

private:
  Session& session;
_cpp_sub_struct_end

_cpp_sub_struct(Hub, QObject)
  Hub();
 ~Hub();

  void post(Task*);

private:
  Session session;

signals:
  void workToDo(shTask);

private:
  void registerMetaTypes();
  bool event(QEvent*);

  QThread thread;
  Worker  worker;
  Q_OBJECT

public:
  void done_long_square(int);
_cpp_struct_end

#endif
// eof
