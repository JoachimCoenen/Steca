/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: refcnt.hpp                                                    $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/util/refcnt.hpp 6     14/07/04 2:06p Ian                $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file  refcnt.hpp
 *  \brief Reference counted objects and pointers.
 *
 *  Reference counted objects and pointers. Objects must be heap-based;
 *  creating of such objects on the stack is prevented by the private destructor.
 */

#ifndef UTIL_REFCNT_HPP_INCLUDED
#define UTIL_REFCNT_HPP_INCLUDED

  #include "../types/types.hpp"
  #include "async.hpp"

  namespace util {
     
//----------------------------------------------------------------------------

  /** The base of self-destructing objects.
    */
  class auto_object
  {
  private:
    static thread::tls& HeapPending();

  protected:

    bool m_isOnHeap;

  public:
    /// overloaded \c new
    static pvoid operator new(size_t sz);

  private:
    static pvoid operator new[](size_t);    ///< prevents use of \c new[]
    static  void operator delete[](pvoid);  ///< prevents use of \c delete []

  protected:
    /// Detects if the object is on a heap and sets a flag accordingly.
    auto_object();
    /// Must have a virtual destructor.
    virtual ~auto_object()
    {
    }
  };
  
//----------------------------------------------------------------------------

  /// Reference counted object.
  class refcnt: public auto_object
  {
    SUPER(auto_object)
    
  private:
    /// reference count
    int  m_cnt;
    
  public:
    /// default
    refcnt()
      : super(), m_cnt(0)
    {
    }

    /// copy
    refcnt(refcnt const&)
      : super(), m_cnt(0)
    {
    }

    /// assignment
    refcnt& operator=(refcnt const& r);
    
    uint inc_refcnt();  ///< increments the count
    uint dec_refcnt();  ///< decrements the count, may destruct the object
    uint qry_refcnt();  ///< returns the count
  };

//----------------------------------------------------------------------------
  
  /// Base class of pointers to reference counted objects
  class refptr_base 
  {
  protected:
    /// the pointed-to object
    refcnt *m_ptr;

  protected:
    refptr_base();
    refptr_base(refptr_base const&);
    refptr_base(refcnt*);
 
  public:
    /// may trigger destruction of the pointed-to object.
    virtual ~refptr_base()
    {
      release();
    }
   
  protected:
    refptr_base& operator=(refptr_base const&);
    refptr_base& operator=(refcnt*);

  public:
    /// releases the pointed-to object; similar to <tt>= NULL</tt> for pointers
    /// May trigger destruction of the pointed-to object.
    void release();
    /// Like: (*this) == NULL ?    
    bool IsNull() const { return NULL==m_ptr; }
    
  private:
    /// increments the reference count
    uint inc()
    {
      assert(m_ptr);
      return m_ptr->inc_refcnt();
    }
  
    /// decrements the reference count
    uint dec()
    {
      assert(m_ptr);
      return m_ptr->dec_refcnt();
    }
  
  protected:
    /// compare
    inline bool operator==(refptr_base const& r)
    {
      return m_ptr == r.m_ptr;
    }
    
    /// compare
    inline bool operator!=(refptr_base const& r)
    {
      return m_ptr != r.m_ptr;
    }
    
    /// compare
    inline bool operator==(refcnt* ptr)
    {
      return m_ptr == ptr;
    }
    
    /// compare
    inline bool operator!=(refcnt* ptr)
    {
      return m_ptr != ptr;
    }
    
  public:
    /// swap
    void swap(refptr_base& r);
  };
  
//----------------------------------------------------------------------------

  /// Reference counted object.
  template <typename T>
  class refptr: public refptr_base
  {
    SUPER(refptr_base)

  public:
    /// no object pointed to (NULL)
    refptr(): super()
    {
    }
  
    /// copy
    refptr(refptr<T> const& r): super(r)
    {
    }
  
    /// takes the adress of a pointed-to object, or NULL
    explicit refptr(refcnt* ptr): super(ptr)
    {
    }
    
    /// assign
    refptr& operator=(refptr<T> const& r)
    {
      return static_cast<refptr&>(super::operator=(r));
    }
  
    /// assign
    refptr& operator=(refcnt* ptr)
    {
      return static_cast<refptr&>(super::operator=(ptr));
    }

    /// a plain pointer
    T* ptr() const 
    {
      return static_cast<T*>(m_ptr);
    }
  
    /// overloaded operator
    T& operator*() const
    {
      assert(m_ptr);
      return *static_cast<T*>(m_ptr);
    }
  
    /// overloaded operator
    T* operator->() const 
    {
      assert(m_ptr);
      return static_cast<T*>(m_ptr);
    }

    /// compare pointed-to objects for identity
    inline bool operator==(refptr<T> const& r)
    {
      return super::operator==(r);
    }
    
    /// compare pointed-to objects for identity
    inline bool operator!=(refptr<T> const& r)
    {
      return super::operator!=(r);
    }

    /// compare pointed-to objects for identity
    inline bool operator==(refcnt* ptr)
    {
      return super::operator==(ptr);
    }
    
    /// compare pointed-to objects for identity
    inline bool operator!=(refcnt* ptr)
    {
      return super::operator!=(ptr);
    }
  };

  /// compare pointed-to objects for identity
  template <typename T>
  inline bool operator==(refcnt* ptr, refptr<T> const& r)
  {
    return r.operator==(ptr);
  }
  
  /// compare pointed-to objects for identity
  template <typename T>
  inline bool operator!=(refcnt* ptr, refptr<T> const& r)
  {
    return r.operator!=(ptr);
  }

//-----------------------------------------------------------------------------

  } // namespace

#endif

//** eof **********************************************************************
