/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: async.hpp                                                     $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/util/async.hpp 8     15/07/04 12:11p Ian                $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file  async.hpp
 *  \brief Asynchronicity support.
 *  Threads, critical sections etc.
 */

#ifndef UTIL_ASYNC_HPP_INCLUDED
#define UTIL_ASYNC_HPP_INCLUDED

  #include "../types/types.hpp"

//----------------------------------------------------------------------------

  #ifdef WIN32
    #define __FASTCALL __fastcall
  #else
    #define __FASTCALL
  #endif

  namespace util {
  
  /** @defgroup atomic Atomic operations
   *  Atomic operations ...
   *  @{
   */

  int __FASTCALL atomic_increment(int*);            ///<atomic increment
  int __FASTCALL atomic_decrement(int*);            ///<atomic decrement
  int __FASTCALL atomic_exchange (int*,int value);  ///<atomic exchange
    
  /** @} */
  
//----------------------------------------------------------------------------

  /// Thread
  class thread
  {
    NO_GEN(thread)
  
  public:
    #define EXC_IDS START_FAILED,CANT_WAIT,SET_FAILED,KEYCREATE_FAILED
    DECLARE_EXC_ROOT
    #undef EXC_IDS

  public:
    /// thread local storage
    class tls
    {
    private:
      pvoid m_key;
        
    public:
      tls() throw(exc);
     ~tls();
     
      void  set(pcvoid) throw(exc);
      pvoid get() const;
    };
    
  public:
    typedef pvoid handle;

  private:
  #ifdef WIN32
    uint    m_id;
  #endif
    handle  m_handle;

    typedef int bint;   // int used as boolean
    bint    m_running;
    bint    m_signaled;
    bint    m_finished;
  
  public:
    thread();
    virtual ~thread();
  
  private:
  #ifdef WIN32
    static uint __stdcall threadproc(pvoid thr);
  #else
    static pvoid threadproc(pvoid thr);
  #endif
    void _body();
  
  protected:
    virtual void body() = 0;
  
  public:
    void start()    throw(exc);
    bool running()  const;
    bool signaled() const;
    bool finished() const;
    void signal();
  
    void wait()     throw(exc);
    
    bool isCurrent() const;

    enum ePriority {PRIORITY_NORMAL, PRIORITY_HIGH, PRIORITY_LOW};
    void setPriority(ePriority);
  };
  
//----------------------------------------------------------------------------

  /// Mutex
  class mutex
  {
    NO_GEN(mutex)

  protected:
    pvoid m_mtx;

  public:
    mutex();
   ~mutex();
    void enter();
    void leave();
  };

//----------------------------------------------------------------------------

  /// Semaphore
  class semaphore
  {
    NO_GEN(semaphore)

  protected:
    pvoid m_sem;

  public:
    semaphore(uint initVal=0);
   ~semaphore();
    void wait();
    void signal();
  };

//----------------------------------------------------------------------------

  /// Critical section;
  class critical_section
  {
    NO_GEN(critical_section)

  protected:
    mutex &m_mutex;

  public:
    critical_section(mutex&);
   ~critical_section();
  };

//-----------------------------------------------------------------------------

  } // namespace

#endif

//** eof **********************************************************************
