/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: types.hpp                                                     $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/types/types.hpp 23    6/10/04 1:37p Peter               $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file  types.hpp
 *  \brief Basic types, typedefs and macros.
 *
 *  Basic types, typedefs and macros that make C++ life easier.
 *  Common include file that should be included everywhere.
 */

#ifndef AX_TYPES_HPP_INCLUDED
#define AX_TYPES_HPP_INCLUDED

  #include "types.h"

//-----------------------------------------------------------------------------
// Macros that help declare classes.

  /** \def SUPER(superClass)
    * Declares the \c super synonym for a superclass.
    */
  #define SUPER(superClass)      typedef superClass super;

  /** \def COPY_CONS(Class)
    * Declares a copy constructor.
    */
  #define COPY_CONS(Class)       Class::Class(Class const& r)
  
  /** \def OP_ASSIGN(Class)
    * Declares an assignment operator.
    */
  #define OP_ASSIGN(Class)       Class& Class::operator= (Class const& r)
  
  /** \def OP_EQ(Class)
    * Declares an equality operator.
    */
  #define OP_EQ(Class)           bool Class::operator== (Class const& r) const
  
  /** \def OP_NE(Class)
    * Declares an inequality operator.
    */
  #define OP_NE(Class)           bool Class::operator!= (Class const& r) const

  /** \def NO_COPY_CONS(Class)
    * Declares a private class copy constructor.
    * If no definition is provided, this disables the copy constructor.
    * Note that a copy constructor may otherwise be generated automatically.
    */
  #define NO_COPY_CONS(Class)   \
    private:                    \
      COPY_CONS(Class);

  /** \def NO_ASSIGN(Class)
    * Declares a private assignment operator.
    * If no definition is provided, this disables the assignment.
    * Note that an assignment operator may otherwise be generated automatically.
    */
  #define NO_ASSIGN(Class)      \
    private:                    \
      OP_ASSIGN(Class);

  /** \def NO_GEN(Class)
    * Ensures that a copy constructor and an assignment operator will not
    * be automatically genereated.
    */
  #define NO_GEN(Class)         \
    NO_COPY_CONS(Class)         \
    NO_ASSIGN(Class)
    

  /** \def COMPARABLE(type)
    * Defines comparison operators ( ==, !=, \<, etc.). The class (C) 
    * must support a method <tt>int Compare(C const& r)</tt> that returns a negative 
    * value if <tt>*this</tt> is less than \c r, a positive value if 
    * <tt>*this</tt> is greater than \c r, and 0 if <tt>*this</tt> and r are 
    * identical.
    */
  #define COMPARABLE(type)                     \
    bool operator==(type const& test) const    \
    {                                          \
        return (bool)(Compare(test) == 0);     \
    }                                          \
                                               \
    bool operator!=(type const& test) const    \
    {                                          \
        return (bool)(Compare(test) != 0);     \
    }                                          \
                                               \
    bool operator< (type const& test) const    \
    {                                          \
        return (bool)(Compare(test) < 0);      \
    }                                          \
                                               \
    bool operator<=(type const& test) const    \
    {                                          \
        return (bool)(Compare(test) <= 0);     \
    }                                          \
                                               \
    bool operator> (type const& test) const    \
    {                                          \
        return (bool)(Compare(test) > 0);      \
    }                                          \
                                               \
    bool operator>=(type const& test) const    \
    {                                          \
        return (bool)(Compare(test) >= 0);     \
    }

//----------------------------------------------------------------------------
// The mother of all exceptions

  /// The mother of all exceptions
  /** The base class of all exception classes. Use \c DECLARE_EXC macro when
   *  declaring a class that needs to throw exceptions. The hierarchy of 
   *  exception classes will thus reflect the hierarchy of classes throwing
   *  exceptions and will allow the exceptions being catched selectively.
   */
  class exc: public exception
  {
  protected:
    /// a numeric exception code
    uint        m_id;
    /// a text annotation
    std::string m_msg;
    
  public:
    /// Overriden exception method.
    virtual __exString what() const
    {
        return m_msg.c_str();
    }

    enum 
    {
      EXC_NONE   = 0,      // unclassified exception
      EXC_SYSTEM,          // system error (something gravely wrong)
      EXC_ASSERT,          // assertion failed
      EXC_NOT_IMPLEMENTED, // feature not (yet) implemented
      EXC_NOT_HERE,        // a place that should not have been reached
      EXC_RUNTIME,         // a runtime check failed
      EXC_USER   = 1000    // user exceptions start here
    }; 

    /// default, optionally specify \c id and \c msg
    exc(uint id=EXC_NONE, pcstr msg=NULL)
      : m_id(id), m_msg(msg ? msg : "")
    {
    }
    
    /// taking \c id and \c string
    exc(uint id, std::string const& msg)
      : m_id(id), m_msg(msg)
    {
    }
    
    /// get the exception id
    uint                Id() const { return m_id; }
    /// get the exception message
    std::string const&  Msg() const { return m_msg; }
    /// a human readable deescription
    std::string         What() const;
  };

  /// raise
  template<class E>
  void throw_exc(E const& e)
  {
    if(!std::uncaught_exception())
      throw e;
  }

  #define DECLARE_EXC_SUPER(superExc)          \
    class exc: public superExc                 \
    {                                          \
    public:                                    \
      enum {EXC_USER = 1000,EXC_IDS};          \
                                               \
      exc(uint id=EXC_NONE,pcstr msg=NULL)     \
        : superExc(id,msg)                     \
      {                                        \
      }                                        \
                                               \
      exc(uint id,std::string const& msg)      \
        : superExc(id,msg)                     \
      {                                        \
      }                                        \
    };

  /** \def DECLARE_EXC
    * Defines an \c exc subclass. Define \c EXC_IDS as a list of exception ids
    * before using \c DECLARE_EXC. The superclass must have declared or inherited
    * its own exception class.
    */
  #define DECLARE_EXC       DECLARE_EXC_SUPER(super::exc)
  /** \def DECLARE_EXC_ROOT
    * Defines an \c exc subclass of the global ::exc class. 
    * Define \c EXC_IDS as a list of exception ids
    * before using \c DECLARE_EXC.
    */
  #define DECLARE_EXC_ROOT  DECLARE_EXC_SUPER(::exc)

  /// Throws an exception
  #define THROW_EXC2(id,msg)                \
    ::throw_exc(exc(exc::id,msg));

  /// Throws an unclassified exception
  #define THROW_EXC(msg)                    \
    THROW_EXC2(EXC_NONE,msg)

//----------------------------------------------------------------------------
/// Debug time macros.
/** Note that since the macros represent compound statements (\c {} ),
  * there is no semicolon needed after a macro is used.
  */

/** \def ASSERT_EXC(expr)
  * In a debug build throws up if \c expr evaluates to 0.
  * In a release build the statement is ignored (eliminated.)
  */

/** \def ASSERT_EXC3(expr,id,msg)
  * In a debug build calls <tt>THROW_EXC2(id,msg)</tt> if \c expr evaluates to 0.
  * In a release build the statement is ignored (eliminated.)
  */

/** \def VALIDATE_EXC(expr)
  * In a debug build throws up if \c expr evaluates to 0.
  * In a release build \c expr is still evaluated for side-effects,
  * but the result is ignored.
  */

/** \def TRACE(tag,text)
  * In a debug build prints a trace to stdout.
  * In a release build the statement is ignored (eliminated.)
  * Typically used as: <<tt>TRACE("tag",sformat("...",...)</tt>.
  */

/** \def NOT_HERE()
  * Use this to mark a place that should not have been reached, as in:
  * \verbatim
  * switch(field)
  * {
  *   case x: ... break;
  *   case y: ... break;
  *   case z: ... break;
  *   default: NOT_HERE()
  * }
  * \endverbatim
  * Ignored in a release build.
  */

/** \def NOT_HERE1(msg)
  * Same as \c NOT_HERE, allows to specify a message text.
  * Ignored in a release build.
  */

/** \def RUNTIME_CHECK_EXC(expr)
  * Throws up if \c expr evaluates to 0, both in debug and release modes.
  */

/** \def RUNTIME_CHECK_EXC3(expr,id,msg)
  * Calls <tt>THROW_EXC2(id,msg)</tt> if \c expr evaluates to 0,
  * both in debug and release modes.
  */

#ifdef _DEBUG

  #define ASSERT_EXC(expr)  \
    {if(!(expr))            \
      ::throw_exc(exc(exc::EXC_ASSERT, "** ASSERT ** " __FILE__ ": " #expr));}

  #define ASSERT_EXC3(expr,id,msg)  \
    {if(!(expr))            \
      THROW_EXC2(id,msg)}

  #define VALIDATE_EXC(expr) ASSERT_EXC(expr)

  #define TRACE(tag,text)   \
    {std::cout << "@ " << tag << " @ " << text << std::endl;}

  #define NOT_HERE() \
    {::throw_exc(exc(exc::EXC_NOT_HERE,""));}

  #define NOT_HERE1(msg) \
    {::throw_exc(exc(exc::EXC_NOT_HERE,msg));}

#else

  #define ASSERT_EXC(expr)            {}
  #define ASSERT_EXC3(expr,id,msg)    {}
  #define VALIDATE_EXC(expr)          {expr;}
  #define TRACE(tag,text)             {}
  #define NOT_HERE()                  {}
  #define NOT_HERE1(msg)              {}

#endif
  
  #define RUNTIME_CHECK_EXC(expr)  \
    {if(!(expr))            \
      ::throw_exc(exc(exc::EXC_RUNTIME));}

  #define RUNTIME_CHECK_EXC2(expr,msg)  \
    {if(!(expr))            \
      THROW_EXC2(exc::EXC_RUNTIME,msg)}

  #define RUNTIME_CHECK_EXC3(expr,id,msg)  \
    {if(!(expr))            \
      THROW_EXC2(id,msg)}

/** \def NOT_IMPLEMENTED(msg)
  * Use this to mark a yet undeveloped code.
  */
  
  #define NOT_IMPLEMENTED(msg) \
    { ::throw_exc(exc(exc::EXC_NOT_IMPLEMENTED,msg));}
  
/** \def SYSTEM_ERROR(msg)
  * Sumting wong.
  */
  
  #define SYSTEM_ERROR(msg) \
    { ::throw_exc(exc(exc::EXC_SYSTEM,msg));}
  

  #define EXEC_AND_TRACE(a) (a)
  /// useful for printing out a gl call sequence
//  #define EXEC_AND_TRACE(a) (a); std::cout << #a << std::endl;

//-----------------------------------------------------------------------------

  #undef min
  /// Templated \c min function.
  template<class T>
  T min(T a, T b)
  {
    return a<b ? a : b;
  }

  #undef max
  /// Templated \c max function.
  template<class T>
  T max(T a, T b)
  {
    return a>b ? a : b;
  }

  #undef abs
  /// Templated \c abs function.
  template<class T>
  T abs(T a)
  {
    return a>=0 ? a : -a;
  }

  /// Templated \c swap function.
  template<class T>
  void swap(T& a, T& b)
  {
      T tmp = a;
      a = b;
      b = tmp;
  }

//-----------------------------------------------------------------------------

  /// like 'sprintf' for std::string
  std::string sformat(pcstr format,...);

  /// trim a string
  std::string trimleft (std::string const& s);
  /// trim a string
  std::string trimright(std::string const& s);
  /// trim a string
  std::string trim     (std::string const& s);

  /// test for empty string
  inline bool empty(pcstr s)
  {
      return !s || !*s;
  }

//-----------------------------------------------------------------------------

#endif

//** eof **********************************************************************
