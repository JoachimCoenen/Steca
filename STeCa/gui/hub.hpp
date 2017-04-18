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

#ifndef GUI_HUB_HPP
#define GUI_HUB_HPP

#include "acts.hpp"
#include <core/session.hpp>
#include <c2/c/ptr.h>

#include <QObject>
#include <QEvent>
#include <QThread>

namespace gui {
//------------------------------------------------------------------------------

struct Hub;

dcl_struct_sub_(Task, QEvent) SHARED
  Task();
 ~Task();

  void set(Hub& hub);
  virtual Task* clone() = 0;

  virtual void work() = 0;
  virtual void done() = 0;

  Hub* hub;
dcl_end

//------------------------------------------------------------------------------
}

Q_DECLARE_METATYPE(gui::Task::sh)

namespace gui {
//------------------------------------------------------------------------------

dcl_struct_sub_(Worker, QObject)
  Worker(Hub&);

  void doWork(Task::sh);

signals:
  void workDone(Task::sh);

private_
  Hub& hub;
  Q_OBJECT
dcl_end

//------------------------------------------------------------------------------

struct Win;

dcl_struct_sub2_(Hub, QObject, core::Session)
  Hub(Win&);
 ~Hub();

  Acts acts;

  void post(Task*);
  void workDone(Task::sh);

signals:
  void doWork(Task::sh);

private_
  void registerMetaTypes();
  bool event(QEvent*);

  QThread thread;
  Worker  worker;
  Q_OBJECT
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
