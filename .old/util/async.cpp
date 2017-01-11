/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: async.cpp                                                     $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/util/async.cpp 6     27/07/04 3:42p Ian                 $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/* Parts of the code in this module were adapted from:
     C++ Portable Types Library (PTypes) 
     Copyright (c) 2001, 2002, 2003 Hovik Melikyan
     http://www.melikyan.com/ptypes/
     http://ptypes.sourceforge.net/

   PTypes is a free library.
*/

  #include "async.hpp"

#ifdef WIN32
  #define _WINSOCKAPI_   // prevent inclusion of winsock.h in windows.h
  #include <windows.h>
  #include <process.h>
#else
  #include <pthread.h>
  #ifndef __bsdi__
    #include <semaphore.h>
  #endif
#endif

  namespace util {
     
//----------------------------------------------------------------------------

#ifdef WIN32

  int __FASTCALL atomic_increment(int* target)
  {
    return ::InterlockedIncrement((long*)target);
  }
  
  int __FASTCALL atomic_decrement(int* target)
  {
    return ::InterlockedDecrement((long*)target);
  }
  
  int __FASTCALL atomic_exchange (int* target,int value)
  {
    return ::InterlockedExchange((long*)target,value);
  }
  
#elif defined(__GNUC__) && (defined(__i386__) || defined(__I386__))
// GNU C compiler on any i386 platform (actually 486+ for xadd)

  int atomic_increment(int* target)
  {
    int temp = 1;
    __asm__ __volatile ("lock ; xaddl %0,(%1)" : "+r" (temp) : "r" (target));
    return temp + 1;
  }
  
  int atomic_decrement(int* target)
  {
    int temp = -1;
    __asm__ __volatile ("lock ; xaddl %0,(%1)" : "+r" (temp) : "r" (target));
    return temp - 1;
  }
  
  int atomic_exchange (int* target,int value)
  {
    __asm__ __volatile ("lock ; xchgl (%1),%0" : "+r" (value) : "r" (target));
    return value;
  }
  
#else
// other platforms: mutex locking

  static mutex atomic_mutex;

  int atomic_increment(int* target)
  {
    critical_section cs(atomic_mutex);
    int r = ++(*target);
    return r;
  }

  int atomic_decrement(int* target)
  {
    critical_section cs(atomic_mutex);
    int r = --(*target);
    return r;
  }

  int atomic_exchange (int* target,int value)
  {
    critical_section cs(atomic_mutex);
    int r = *target;
    *target = value;
    return r;
  }
  
#endif

//----------------------------------------------------------------------------

  thread::thread()
    : 
    #ifdef WIN32
      m_id(0),
    #endif
      m_handle(NULL), 
      m_running(false), m_signaled(false), m_finished(false)
  {
  }

  thread::~thread()
  {
  #ifdef WIN32
    CloseHandle(m_handle);
  #endif
  }

  void thread::_body()
  {
    body();
    m_finished = true;
  }
  
#ifdef WIN32
  uint __stdcall thread::threadproc(pvoid thr)
#else
  pvoid thread::threadproc(pvoid thr)
#endif
  {
    assert(NULL!=thr);
    ((thread*)thr)->_body();
    return 0;
  }

  void thread::start() throw(thread::exc)
  {
    if(!atomic_exchange(&m_running,true))
    {
    #ifdef WIN32
      m_handle = (handle)_beginthreadex(NULL,0,threadproc,this,0,&m_id);
      RUNTIME_CHECK_EXC3(m_handle, START_FAILED, "thread::start() failed")

    #else
      pthread_attr_t attr;
      pthread_attr_init(&attr);
      pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

      if(0 != pthread_create((pthread_t*)&m_handle,&attr,threadproc,this))
        THROW_EXC2(START_FAILED,"thread::start() failed")

      pthread_attr_destroy(&attr);
    #endif
    }
  }

  bool thread::running() const
  {
    return false!=m_running;
  }

  bool thread::signaled() const
  {
    return false!=m_signaled;
  }

  bool thread::finished() const
  {
    return false!=m_finished;
  }

  void thread::signal()
  {
    m_signaled = true;
  }

  void thread::wait() throw(thread::exc)
  {
    if(atomic_exchange(&m_finished,true))
      return;

    if(isCurrent())
      THROW_EXC2(CANT_WAIT,"thread::wait() cannot wait on itself")

  #ifdef WIN32
    WaitForSingleObject(m_handle,INFINITE);
    CloseHandle(m_handle);
  #else
    pthread_join  ((pthread_t)m_handle,NULL);
    pthread_detach((pthread_t)m_handle);
  #endif

    m_handle = NULL;
  }

  bool thread::isCurrent() const
  {
  #ifdef WIN32
    return GetCurrentThreadId() == m_id;
  #else
    return 0!= pthread_equal(pthread_self(),(pthread_t)m_handle);
  #endif
  }

  void thread::setPriority(ePriority p)
  {
  #ifdef WIN32
    int pri = THREAD_PRIORITY_NORMAL;
    switch(p)
    {
    case PRIORITY_HIGH:
      pri = THREAD_PRIORITY_ABOVE_NORMAL;
      break;

    case PRIORITY_LOW:
      pri = THREAD_PRIORITY_BELOW_NORMAL;
      break;
    }

    SetThreadPriority(m_handle,pri);

  #else
    #error not implemented
  #endif
  }

//----------------------------------------------------------------------------

  thread::tls::tls() throw(thread::exc)
    : m_key(NULL)
  {
  #ifdef WIN32
    m_key = (pvoid)TlsAlloc();
    #define M_KEY ((DWORD)m_key)
  #else
    m_key = new pthread_key_t();
    #define M_KEY ((pthread_key_t*)m_key)
    if(0!=pthread_key_create(M_KEY,NULL))
    {
      delete M_KEY;
      m_key = NULL;
      THROW_EXC2(KEYCREATE_FAILED,"thread::tls::tls() failed")
    }
  #endif
  }
  
  thread::tls::~tls()
  {
  #ifdef WIN32
    TlsFree(M_KEY);
  #else
    if(m_key)
      pthread_key_delete(*M_KEY);
  #endif
  }

  void thread::tls::set(pcvoid value) throw(exc)
  {
  #ifdef WIN32
    if(!TlsSetValue(M_KEY,(pvoid)value))
  #else
    if(0!=pthread_setspecific(*M_KEY,value))
  #endif
      THROW_EXC2(SET_FAILED,"thread::tls::set() failed")
  }
  
  pvoid thread::tls::get() const
  {
  #ifdef WIN32
    return TlsGetValue(M_KEY);
  #else
    return pthread_getspecific(*M_KEY); 
  #endif
  }
  
//----------------------------------------------------------------------------
/// Mutex

  mutex::mutex()
    : m_mtx(NULL)
  {
  #ifdef WIN32
    m_mtx = new CRITICAL_SECTION();
    #define M_MTX ((CRITICAL_SECTION*)m_mtx)
    InitializeCriticalSection(M_MTX);
  #else
    m_mtx = new pthread_mutex_t();
    #define M_MTX ((pthread_mutex_t*)m_mtx)
    pthread_mutex_init(M_MTX, 0);
  #endif
  }

  mutex::~mutex()
  {
  #ifdef WIN32
    DeleteCriticalSection(M_MTX);
  #else
    pthread_mutex_destroy(M_MTX);
  #endif
    delete M_MTX;
  }

  void mutex::enter()
  {
  #ifdef WIN32
    EnterCriticalSection(M_MTX);
  #else
    pthread_mutex_lock(M_MTX);
  #endif
  }

  void mutex::leave()
  {
  #ifdef WIN32
    LeaveCriticalSection(M_MTX);
  #else
    pthread_mutex_unlock(M_MTX);
  #endif
  }

//----------------------------------------------------------------------------
/// Semaphore

  semaphore::semaphore(uint initialCount)
   : m_sem(NULL)
  {
  #ifdef WIN32
    m_sem = CreateSemaphore(NULL,initialCount,LONG_MAX,NULL);
    #define M_SEM ((HANDLE)m_sem)
  #else
    #error not implemented
  #endif
  }

  semaphore::~semaphore()
  {
  #ifdef WIN32
    CloseHandle(M_SEM);
  #else
    #error not implemented
  #endif
  }

  void semaphore::wait()
  {
  #ifdef WIN32
     WaitForSingleObject(M_SEM,INFINITE);
  #else
    #error not implemented
  #endif
  }

  void semaphore::signal()
  {
  #ifdef WIN32
    ReleaseSemaphore(M_SEM,1,NULL);
  #else
    #error not implemented
  #endif
  }

//----------------------------------------------------------------------------

  critical_section::critical_section(mutex& m)
    : m_mutex(m)
  {
    m_mutex.enter();
  }

  critical_section::~critical_section()
  {
    m_mutex.leave();
  }

//-----------------------------------------------------------------------------

  } // namespace

//** eof **********************************************************************
