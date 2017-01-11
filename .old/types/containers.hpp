/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: containers.hpp                                                $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/types/containers.hpp 9     6/07/04 2:42p Peter          $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file  containers.hpp
 *  \brief Container classes.
 *
 *  Useful derived container classes.
 */

#ifndef AX_CONTAINER_HPP_INCLUDED
#define AX_CONTAINER_HPP_INCLUDED

  #include "types.hpp"
  #include <vector>
  #include <algorithm>
  #include <queue>

//-----------------------------------------------------------------------------


  /** A vector of owned objects (T*). Owns the objects and destroys them when 
   *  they are removed by \c clear(), \c pop_back(), ~(). etc.
   *  Beware: <tt>std::vector</tt> destructor is not virtual!
   *  An alternative solution would use vector<auto_ptr<T> >
   */

  template <class T>
  class auto_vector: public std::vector<T*>
  {
      SUPER(std::vector<T*>)

  public:
      ~auto_vector()
      {
          for(iterator i=begin(); i<end(); i++)
          {
              delete *i;
          }
      }

      iterator erase(iterator it)
      {
          delete *it;
          return super::erase(it);
      }

      iterator erase(iterator first, iterator last)
      {
          for(iterator i=first; i<last; i++)
          {
              delete *i;
          }
          return super::erase(first, last);
      }

      void pop_back()
      {
          delete *rbegin();
          super::pop_back();
      }

      iterator find(T const& val)
      {
          return std::find(begin(),end(),&val);
      }
  };


//-----------------------------------------------------------------------------

/** A queue of owned objects (T*). Owns the objects and destroys them when 
   *  they are removed by ~(), pop_front() or pop_back().
   *  Beware: <tt>std::deque</tt> destructor is not virtual!
   *  An alternative solution would use queue<auto_ptr<T> >
   */

  template <class T>
  class auto_queue: public std::deque<T*> //pm>>> didn't compile with queue
  {
      SUPER(std::deque<T*>)

  public:
      ~auto_queue()
      {
          for(iterator i=begin(); i<end(); i++)
          {
              delete *i;
          }
      }

      void pop_front()
      {
          delete front();
          super::pop_front();
      }

      void pop_back()
      {
          delete back();
          super::pop_back();
      }

  };

//-----------------------------------------------------------------------------

#endif

//** eof **********************************************************************
