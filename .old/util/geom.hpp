/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: geom.hpp                                                      $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/util/geom.hpp 18    8/10/04 5:14p Dan                   $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file  geom.hpp
 *  \brief Templated geometry classes
 *
 *  Debug version performs boundary checking. Release version has little overhead
 */

#ifndef UTIL_GEOM_HPP_INCLUDED
#define UTIL_GEOM_HPP_INCLUDED

  #include "../types/types.hpp"

  namespace util {
     
//----------------------------------------------------------------------------

  /// vector1: an x single, provided for consistency wioth vector3|4 reasons
  template<typename T>
  struct vector1
  {
    #define EXC_IDS OUT_OF_BOUNDS
    DECLARE_EXC_ROOT
    #undef EXC_IDS

    T x;

    vector1()
     : x(T(0))
    {
    }

    vector1(T x_)
     : x(x_)
    {
    }

    inline T& operator[] (uint const i)
    {
      ASSERT_EXC3(i<1,OUT_OF_BOUNDS,"vector1: index out of bounds")
      return *((&x) + i);
    }

    inline T const& operator[] (uint const i) const
    {
      return const_cast<vector1*>(this)->operator[](i);
    }

    COMPARABLE(vector1)
    int Compare(vector1 const& r) const
    {
      if(x<r.x) return -1;
      if(x>r.x) return +1;
      return 0;
    }
  };

//----------------------------------------------------------------------------
  /// vector2: a uv double 
  template<typename T>
  struct vector2
  {
    #define EXC_IDS OUT_OF_BOUNDS
    DECLARE_EXC_ROOT
    #undef EXC_IDS

    T u, v;

    vector2()
     : u(T(0)), v(T(0))  // dc>>> This assumes that T is a numeric type, or at least has a default constructor
                         // that takes an int.
    {
    }

    vector2(T init)
     : u(init), v(init)
    {
    }

    vector2(T u_, T v_)
     : u(u_), v(v_)
    {
    }

    inline T& operator[] (uint const i)
    {
      ASSERT_EXC3(i<2,OUT_OF_BOUNDS,"vector2: index out of bounds")
      return *((&u) + i);
    }

    inline T const& operator[] (uint const i) const
    {
      return const_cast<vector2*>(this)->operator[](i);
    }

    friend inline vector2<T> operator-( vector2<T> const& lhs, 
                                        vector2<T> const& rhs )
    {
        return vector2<T>(lhs.u-rhs.v, lhs.u-rhs.v, lhs.u-rhs.v); 
    }


    COMPARABLE(vector2)
    int Compare(vector2 const& r) const
    {
      if(u<r.u) return -1;
      if(u>r.u) return +1;
      if(v<r.v) return -1;
      if(v>r.v) return +1;
      return 0;
    }
  };
  //----------------------------------------------------------------------------


  /// vector3: an xyz triple 
  template<typename T>
  struct vector3
  {
    #define EXC_IDS OUT_OF_BOUNDS
    DECLARE_EXC_ROOT
    #undef EXC_IDS

    T x, y, z;  // w;   // might be faster with 4 floats 
                        //(if they get 128-bit aligned) (will be needed for SSE)

    vector3()
     : x(T(0)), y(T(0)), z(T(0)) 
    {
    }

    vector3(T x_)
     : x(x_), y(x_), z(x_) 
    {
    }

    vector3(T x_, T y_, T z_)
     : x(x_), y(y_), z(z_) 
    {
    }

    inline T& operator[] (uint const i)
    {
      ASSERT_EXC3(i<3,OUT_OF_BOUNDS,"vector3: index out of bounds")
      return *((&x) + i);
    }

    inline T const& operator[] (uint const i) const
    {
      return const_cast<vector3*>(this)->operator[](i);
    }

    friend inline vector3<T> operator-( vector3<T> const& lhs, 
                                        vector3<T> const& rhs )
    {
        return vector3<T>(lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z); 
    }


    COMPARABLE(vector3)
    int Compare(vector3 const& r) const
    {
      if(x<r.x) return -1;
      if(x>r.x) return +1;
      if(y<r.y) return -1;
      if(y>r.y) return +1;
      if(z<r.z) return -1;
      if(z>r.z) return +1;
      return 0;
    }
  };

//----------------------------------------------------------------------------

  /// vector4: an xyzw quadruple 
  template<typename T>
  struct vector4
  {
    #define EXC_IDS OUT_OF_BOUNDS
    DECLARE_EXC_ROOT
    #undef EXC_IDS

    T x, y, z, w;

    vector4()
     : x(T(0)), y(T(0)), z(T(0)), w(T(0)) 
    {
    }

    vector4(T x_)
     : x(x_), y(x_), z(x_), w(x_)
    {
    }

    vector4(T x_, T y_, T z_, T w_)
     : x(x_), y(y_), z(z_), w(w_)
    {
    }

    inline T& operator[] (uint const i)
    {
      ASSERT_EXC3(i<4,OUT_OF_BOUNDS,"vector4: index out of bounds")
      return *((&x) + i);
    }

    inline T const& operator[] (uint const i) const
    {
      return const_cast<vector4*>(this)->operator[](i);
    }

    COMPARABLE(vector4)
    int Compare(vector4 const& r) const
    {
      if(x<r.x) return -1;
      if(x>r.x) return +1;
      if(y<r.y) return -1;
      if(y>r.y) return +1;
      if(z<r.z) return -1;
      if(z>r.z) return +1;
      if(w<r.w) return -1;
      if(w>r.w) return +1;
      return 0;
    }
  };

//-----------------------------------------------------------------------------

  /// insert into a stream
  template<class T>
  std::ostream& operator<<(std::ostream& os, vector1<T> const& v)
  {
    return os << v.x;
  }

  /// insert into a stream
  template<class T>
  std::ostream& operator<<(std::ostream& os, vector3<T> const& v)
  {
    return os << v.x << ',' << v.y << ',' << v.z;
  }

  /// insert into a stream
  template<class T>
  std::ostream& operator<<(std::ostream& os, vector4<T> const& v)
  {
    return os << v.x << ',' << v.y << ',' << v.z << ',' << v.w;
  }

//-----------------------------------------------------------------------------

  typedef vector1<float> float1;
  typedef vector2<float> float2;
  typedef vector3<float> float3;  
  typedef vector4<float> float4;

  typedef vector2<int>   int2;
  typedef vector3<int>   int3;
  typedef vector4<int>   int4;
  
  typedef vector2<uint>  uint2;
  typedef vector3<uint>  uint3;
  typedef vector4<uint>  uint4;

//-----------------------------------------------------------------------------

} // namespace

#endif

//** eof **********************************************************************
