/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: array.hpp                                                     $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/util/array.hpp 27    7/10/04 2:54p Dan                  $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file  array.hpp
 *  \brief Templated dynamic array class
 *
 *  Debug version performs boundary checking. Release version has little overhead
 */

#ifndef UTIL_ARRAY_HPP_INCLUDED
#define UTIL_ARRAY_HPP_INCLUDED

  #include "../types/types.hpp"
  #include "geom.hpp"

  namespace util {
     
//----------------------------------------------------------------------------

  /// One-dimensional safe array
  template<typename T>
  class array1d
  {
  protected:
    T    *m_data;
    uint  m_size;

  public:
    #define EXC_IDS OUT_OF_BOUNDS
    DECLARE_EXC_ROOT
    #undef EXC_IDS

  public:
    array1d(uint size): m_data(NULL), m_size(size)
    {
      m_data = new T[m_size];
    }

    array1d(uint size, T const& init): m_data(NULL), m_size(size)
    {
      m_data = new T[m_size];
      set(init);
    }

    array1d(uint size, T* data, bool copy=true) : m_data(NULL), m_size(size)
    {
        if(copy)
        {
            m_data = new T[m_size];
            memcpy(m_data, data, sizeof T * m_size);
        }
        else
        {
            m_data = data;
        }
    }

    array1d(array1d const& r): m_data(NULL), m_size(0)
    {
      *this = r;
    }

    virtual~ array1d()
    {
      delete [] m_data;
    }

    array1d& operator= (array1d const& r)
    {
      delete [] m_data;

      m_size = r.m_size;
      m_data = new T[m_size];
      memcpy(m_data, r.m_data, sizeof T * m_size);

      return *this;
    }

    array1d& set(T const& init)
    {
      for(uint i = 0; i<m_size; i++)
        m_data[i] = init;

      return *this;
    }

    bool operator== (array1d const& r) const
    {
      return (m_size==r.m_size) && (0==memcmp(m_data,r.m_data,m_size));
    }

    bool operator!= (array1d const& r) const
    {
      return !operator==(r);
    }

    inline T& operator() (uint i)
    {
      ASSERT_EXC3(i<m_size,OUT_OF_BOUNDS,"array: index out of bounds")
      return m_data[i];
    }

    inline T const& operator() (uint i) const
    {
      return const_cast<array1d*>(this)->operator()(i);
    }

    inline T& operator[] (uint i)
    {
      return operator()(i);
    }

    inline T const& operator[] (uint i) const
    {
      return const_cast<array1d*>(this)->operator[](i);
    }

    T* ptr()
    {
      return m_data;
    }

    T const* ptr() const
    {
      return const_cast<array1d*>(this)->ptr();
    }

    T* ptr(uint i)
    {
      return m_data + i;
    }

    T const* ptr(uint i) const
    {
      return const_cast<array1d*>(this)->ptr(i);
    }

    T* detach()
    {
      T* data = m_data;
      m_data = NULL;
      return data;
    }

    uint size() const { return m_size; }
  };

//-----------------------------------------------------------------------------

  /// insert into a stream
  template<class T>
  std::ostream& operator<<(std::ostream& os, array1d<T> const& a)
  {
    for(uint i=0; i<a.size(); i++)
    {
      if(i>0)
        os << ',';
      os << a[i];
    }
    return os;
  }

//----------------------------------------------------------------------------

  /// Two-dimensional safe array
  template<typename T>
  class array2d: public array1d<T>
  {
    SUPER(array1d<T>)

  protected:
    uint m_size2;

  public:
    array2d(uint size1, uint size2): super(size1*size2), m_size2(size2)
    {
    }

    array2d(uint size1, uint size2, T const& init): super(size1*size2, init), m_size2(size2)
    {
    }

    array2d(uint size1, uint size2, T* data, bool copy=true) : super(size1*size2, data, copy), m_size2(size2)
    {
    }

    array2d(array2d const& r): super(r), m_size2(r.m_size2)
    {
    }

    array2d& operator= (array2d const& r)
    {
      m_size2 = r.m_size2;
      return static_cast<array2d&>(super::operator=(r));
    }

    array2d& set(T const& init)
    {
      super::set(init);
      return *this;
    }

    bool operator== (array2d const& r) const
    {
      return (m_size2==r.m_size2) && super::operator==(r);
    }

    bool operator!= (array2d const& r) const
    {
      return !operator==(r);
    }

    inline T& operator() (uint i)
    {
      return super::operator()(i);
    }

    inline T const& operator() (uint i) const
    {
      return super::operator()(i);
    }

    inline T& operator() (uint i,uint j)
    {
      ASSERT_EXC3(j<m_size2,OUT_OF_BOUNDS,"array2d: index out of bounds")
      return super::operator()(i*m_size2+j);
    }

    inline T const& operator() (uint i,uint j) const
    {
      return const_cast<array2d*>(this)->operator()(i,j);
    }

    T* ptr()
    {
      return super::ptr();
    }

    T const* ptr() const
    {
      return super::ptr();
    }

    T* ptr(uint i)
    {
      return super::ptr(i);
    }

    T const* ptr(uint i) const
    {
      return super::ptr(i);
    }

    T* ptr(uint i, uint j)
    {
      return super::ptr(i*m_size2+j);
    }

    T const* ptr(uint i, uint j) const
    {
      return const_cast<array2d*>(this)->ptr(i,j);
    }

    uint size1() const { return super::size()/m_size2; }
    uint size2() const { return m_size2;               }
  };


//-----------------------------------------------------------------------------

  /// insert into a stream
  template<class T>
  std::ostream& operator<<(std::ostream& os, array2d<T> const& a)
  {
    for(uint i=0; i<a.size1(); i++)
    {
      os << std::endl;
      for(uint j=0; j<a.size2(); j++)
      {
        if(j>0)
          os << ',';
        os << a(i,j);
      }
    }
    return os;
  }

//----------------------------------------------------------------------------

  /// Three-dimensional safe array
  template<typename T>
  class array3d: public array2d<T>
  {
    SUPER(array2d<T>)

  protected:
    uint m_size3;

  public:
    array3d(uint size1, uint size2, uint size3): super(size1,size2*size3), m_size3(size3)
    {
    }

    array3d(uint3 const& size)
        :super(size.x,size.y*size.z), m_size3(size.z)
    {
    }

    array3d(uint size1, uint size2, uint size3, T const& init)
      : super(size1,size2*size3, init), m_size3(size3)
    {
    }

    array3d(uint size1, uint size2, uint size3, T* data, bool copy=true)
        : super(size1,size2*size3, data, copy), m_size3(size3)
    {
    }

    array3d(array3d const& r): super(r), m_size3(r.m_size3)
    {
    }

    array3d& operator= (array3d const& r)
    {
      m_size3 = r.m_size3;
      super::operator=(r);
    }

    array3d& set(T const& init)
    {
      super::set(init);
      return *this;
    }

    bool operator== (array3d const& r) const
    {
      return (m_size3==r.m_size3) && super::operator==(r);
    }

    bool operator!= (array3d const& r) const
    {
      return !operator==(r);
    }

    inline T& operator() (uint i)
    {
      return super::operator()(i);
    }

    inline T const& operator() (uint i) const
    {
      return super::operator()(i);
    }

    inline T& operator() (uint i, uint j)
    {
      return super::operator()(i,j);
    }

    inline T const& operator() (uint i, uint j) const
    {
      return super::operator()(i,j);
    }

    inline T& operator() (uint i,uint j,uint k)
    {
      ASSERT_EXC3(k<m_size3,OUT_OF_BOUNDS,"array3d: index out of bounds")
      return super::operator()(i,j*m_size3+k);
    }

    inline T const& operator() (uint i,uint j,uint k) const
    {
      return const_cast<array3d*>(this)->operator()(i,j,k);
    }

    T* ptr()
    {
      return super::ptr();
    }

    T const* ptr() const
    {
      return super::ptr();
    }

    T* ptr(uint i)
    {
      return super::ptr(i);
    }

    T const* ptr(uint i) const
    {
      return super::ptr(i);
    }

    T* ptr(uint i, uint j)
    {
      return super::ptr(i,j);
    }

    T const* ptr(uint i, uint j) const
    {
      return super::ptr(i,j);
    }

    T* ptr(uint i, uint j, uint k)
    {
      return super::ptr(i,j*m_size3+k);
    }

    T const* ptr(uint i, uint j, uint k) const
    {
      return const_cast<array3d*>(this)->ptr(i,j,k);
    }

    uint size2() const { return super::size2()/m_size3; }
    uint size3() const { return m_size3;                }
  }; // array3d


//-----------------------------------------------------------------------------

  /// insert into a stream
  template<class T>
  std::ostream& operator<<(std::ostream& os, array3d<T> const& a)
  {
    for(uint i=0; i<a.size1(); i++)
    {
      for(uint j=0; j<a.size2(); j++)
      {
        os << std::endl;
        if(j>0)
          os << "  "; // indent
        for(uint k=0; k<a.size3(); k++)
        {
          if(k>0)
            os << ',';
          os << a(i,j,k);
        }
      }
    }
    return os;
  }

//-----------------------------------------------------------------------------

  /// Numeric ops
  template<class N>
  void iota(array1d<N>& a, N const step)
  {
    N *p = a.ptr();
    for(uint i=0, iSize=a.size(); i<iSize; i++)
      *(p++) = i*step;
  }

  template<class N>
  inline void iota(array2d<N>& a, N const step)
  {
    iota((array1d<N>&)a,step);
  }

  template<class N>
  inline void iota(array3d<N>& a, N const step)
  {
    iota((array1d<N>&)a,step);
  }

  template<class N>
  void iota(array1d<N>& a)
  {
    iota(a, N(1));
  }

  template<class N>
  void iota(array2d<N>& a)
  {
    iota(a, N(1));
  }

  template<class N>
  void iota(array3d<N>& a)
  {
    iota(a, N(1));
  }

  template<class N>
  bool almostEqual(array1d<N> const& a, array1d<N> const& b, N const eps)
  {
    if(a.size() != b.size())
      return false;

    N const *pa = a.ptr(), *pb = b.ptr();

    for(uint i=0, iSize=a.size(); i<iSize; i++)
    {
      if(abs((*(pa++)) - (*(pb++))) > eps)
        return false;
    }

    return true;
  }

  template<class N>
  bool almostEqual(array2d<N> const& a, array2d<N> const& b, N const eps)
  {
    return almostEqual((array1d<N> const&)a, (array1d<N> const&)b,eps);
  }

  template<class N>
  bool almostEqual(array3d<N> const& a, array3d<N> const& b, N const eps)
  {
    return almostEqual((array1d<N> const&)a, (array1d<N> const&)b,eps);
  }

//-----------------------------------------------------------------------------

  } // namespace

#endif

//** eof **********************************************************************
