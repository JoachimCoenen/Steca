/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: timer.cpp                                                     $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/util/timer.cpp 5     30/07/04 2:32p Ian                 $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file timer.cpp
 */

  #include "timer.hpp"

  namespace util {

//-----------------------------------------------------------------------------

  Timer::Timer(bool autoStart)
    : m_autoStart(autoStart), 
      m_isRunning(false)
  {
    Reset();
  }

//-----------------------------------------------------------------------------

  void Timer::Reset()
  {
    Finish();
    m_start.QuadPart       = 0;
    m_finish.QuadPart      = 0;
    m_accumulated.QuadPart = 0;

    QueryPerformanceFrequency(&m_freq);
    if(m_autoStart)
      Start();
  }

//-----------------------------------------------------------------------------

  void Timer::Start()
  {
    QueryPerformanceCounter(&m_start);
    m_finish    = m_start;
    m_isRunning = true;
  }

//-----------------------------------------------------------------------------

  void Timer::Finish()
  {
    QueryPerformanceCounter(&m_finish);
    m_isRunning = false;
  }

//-----------------------------------------------------------------------------

  void Timer::Accumulate()
  {
    m_accumulated.QuadPart += (m_finish.QuadPart - m_start.QuadPart);
  }

//-----------------------------------------------------------------------------

  double Timer::Time() const
  {
    LARGE_INTEGER f;
    if(m_isRunning)
      QueryPerformanceCounter(&f);
    else
      f.QuadPart = m_finish.QuadPart;
    return double(f.QuadPart - m_start.QuadPart)/double(m_freq.QuadPart);
  }

//-----------------------------------------------------------------------------

  double Timer::AccumulatedTime() const
  {
    return double(m_accumulated.QuadPart)/double(m_freq.QuadPart);
  }

//-----------------------------------------------------------------------------

  std::ostream& operator<<(std::ostream& os, Timer const& timer)
  {
    return os << timer.Time();
  }

//-----------------------------------------------------------------------------

  } // namespace

//** eof **********************************************************************
