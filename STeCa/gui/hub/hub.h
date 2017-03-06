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
#include <c2/c/lib/ptr.h>

#include <QObject>
#include <QEvent>
#include <QThread>

namespace gui {
//------------------------------------------------------------------------------

struct Hub;

def_struct_sub (Task, QEvent)
  using Session = core::Session;

  Task();
 ~Task();

  void set(Hub& hub, Session& session);
  virtual Task* clone() = 0;

  virtual void work() = 0;
  virtual void done() = 0;

  Hub* hub; Session* session;
def_struct_end

typedef c::shared<Task> shTask;
//------------------------------------------------------------------------------
}

Q_DECLARE_METATYPE(gui::shTask)

namespace gui {
//------------------------------------------------------------------------------

def_struct_sub (Worker, QObject)
  using Session = core::Session;

  Worker(Session&);

  void doWork(shTask);

signals:
  void workDone(shTask);

private:
  Session& session;
  Q_OBJECT
def_struct_end

//------------------------------------------------------------------------------

def_struct_sub (Hub, QObject)
  using Session = core::Session;

  Hub();
 ~Hub();

  void post(Task*);
  void workDone(shTask);

signals:
  void doWork(shTask);

private:
  Session session;

private:
  void registerMetaTypes();
  bool event(QEvent*);

  QThread thread;
  Worker  worker;
  Q_OBJECT
def_struct_end

//------------------------------------------------------------------------------

def_struct (RefHub)
  RefHub(Hub&);

protected:
  Hub& hub;
def_struct_end

//------------------------------------------------------------------------------
}
#endif
// eof
