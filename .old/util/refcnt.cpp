/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: refcnt.cpp                                                    $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/util/refcnt.cpp 4     23/06/04 10:49a Ian               $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file refcnt.cpp
 */

  #include "refcnt.hpp"

  namespace util {

//-----------------------------------------------------------------------------

  thread::tls& auto_object::HeapPending()
  {
    // must be done this way: ensure m_heapPending is constructed before used
    // while constructing global static :refcnt objects

    static thread::tls m_heapPending;
    return m_heapPending;
  }
  
  pvoid auto_object::operator new(size_t sz)
  {
    thread::tls& heapPending = HeapPending();
    uint *hp = (uint*)heapPending.get();
    if(NULL==hp)
    {
      hp = new uint(0);
      heapPending.set(hp);
    }
    
    ++(*hp);
    return ::operator new(sz);
  }
  
  auto_object::auto_object()
    : m_isOnHeap(true)
  {
    thread::tls& heapPending = HeapPending();
    uint *hp = (uint*)heapPending.get();
    if(NULL==hp || 0==*hp)
      m_isOnHeap = false;
    else
      --(*hp);
  }
  
//----------------------------------------------------------------------------

  refcnt& refcnt::operator=(refcnt const& r)
  {
    super::operator=(r);
    return *this;
  }

  uint refcnt::inc_refcnt()
  {
    return atomic_increment(&m_cnt);
  }

  uint refcnt::dec_refcnt()
  {
    uint cnt = atomic_decrement(&m_cnt);
    if(m_isOnHeap && (0==cnt))
      delete this;
    return cnt;
  }

  uint refcnt::qry_refcnt()
  {
    return m_cnt;
  }

//----------------------------------------------------------------------------

  refptr_base::refptr_base() : m_ptr(NULL)
  {
  }

  refptr_base::refptr_base(refptr_base const& r): m_ptr(r.m_ptr)
  {
    if(m_ptr) 
      m_ptr->inc_refcnt(); 
  }

  refptr_base::refptr_base(refcnt* ptr): m_ptr(ptr)
  {
    if(m_ptr) 
      m_ptr->inc_refcnt(); 
  }
  
  refptr_base& refptr_base::operator=(refptr_base const& r) 
  {
    if(m_ptr!=r.m_ptr)
    {
      if(m_ptr)
        m_ptr->dec_refcnt();
      m_ptr = r.m_ptr;
      if(m_ptr)
        m_ptr->inc_refcnt();
    }
  
    return *this;
  }
  
  refptr_base& refptr_base::operator=(refcnt* ptr) 
  {
    release();
    
    m_ptr = ptr;
    if(m_ptr)
      m_ptr->inc_refcnt();
      
    return *this;
  }
  
  void refptr_base::release()
  {
    if(m_ptr)
      m_ptr->dec_refcnt();
      
    m_ptr = NULL;
  }
  
  void refptr_base::swap(refptr_base& r)
  {
    ::swap(m_ptr,r.m_ptr);
  }

//-----------------------------------------------------------------------------

  } // namespace

//** eof **********************************************************************
