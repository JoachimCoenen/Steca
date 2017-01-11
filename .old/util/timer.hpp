/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: timer.hpp                                                     $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/util/timer.hpp 6     30/07/04 2:32p Ian                 $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file  timer.hpp
 *  \brief [TODO: brief file comment here].
 *
 *  [TODO: verbose file comment]
 */

#ifndef UTIL_TIMER_HPP_INCLUDED
#define UTIL_TIMER_HPP_INCLUDED

  #include "../types/types.hpp"
  #include <windows.h>
  #include <iostream>

  namespace util {

//-----------------------------------------------------------------------------

  /// High-resolution performance counter.
  class Timer
  {
    NO_GEN(Timer)

  public:
    /// construct
    Timer(bool autoStart=false);
    /// start fresh
    void Reset();

    /// take a (start) time snaphot 
    void Start();
    /// take a (finish) time snaphot 
    void Finish();
    /// add (finish-start) to accumulated time
    void Accumulate();

    /// between (auto)Start() and Stop()
    bool IsRunning() const { return m_isRunning; }

    /// if IsRunning(), return (now-start), otherwise (finish-start)
    double Time()             const;
    /// total Accumulate()d
    double AccumulatedTime()  const;

  private:
    /// if true, Start() is called implicitly when an object is created or Reset()
    bool m_autoStart;
    /// true between Start() and Stop()
    bool m_isRunning;
    /// Start() time
    LARGE_INTEGER m_start;
    /// Stop() time
    LARGE_INTEGER m_finish;
    /// accumulated time
    LARGE_INTEGER m_accumulated;
    /// performance timer frequency
    LARGE_INTEGER m_freq;
  };

  /// insert into a stream
  std::ostream& operator<<(std::ostream& os, Timer const& timer);

//-----------------------------------------------------------------------------

  } // namespace

#endif

//** eof **********************************************************************
