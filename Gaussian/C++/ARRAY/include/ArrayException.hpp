#ifndef ArrayException_h
#define ArrayException_h
#include <iostream>
using namespace std;
namespace Arrays {
///////////////////////////////////////////////////////////////////////////////////
/// Base class for ArrayException.
/// Wrap messages.
///////////////////////////////////////////////////////////////////////////////////
struct BaseExept{
  BaseExept()
  {
    cerr<<endl<<"============================";
    cerr<<" Array Exception ";
          cerr<<"============================"<<endl;
#ifdef DEBUG
	  cout<<endl<<"You are in DEBUG mode."<<endl;
#endif
  }
};
////////////////////////////////////////////////////////////////////////////////
///
/// A simple exception class.
/// Instantiate it with 1 to 10 arguments. All arguments must be printable,
/// and will actually be printed.
///
//// \brief an all-purpose exception class.
////////////////////////////////////////////////////////////////////////////////
  class ArrayException: BaseExept
{
public:
  //! O argument
  // template<class A> ArrayException()
  // {}
  //! 1 argument
  template<class A> ArrayException(A x) : BaseExept()
    {
      cerr<<endl<<x<<endl<<endl;
    }
  //! 2 arguments
  template<class A,class B> ArrayException(A x,B y) : BaseExept()
    {
      cerr<<endl<<x<<" "<<y<<endl<<endl;
    }
  //! 3 arguments
  template<class A,class B,class C> ArrayException(A x,B y,C z) : BaseExept()
    {
      cerr<<endl<<x<<" "<<y<<" "<<z<<endl<<endl;
    }
  //! 4 arguments
  template<class A,class B,class C,class D> ArrayException(A x,B y,C z,D a): 
    BaseExept()
    {
      cerr<<endl<<x<<" "<<y<<" "<<z<<" "<<a<<endl<<endl;
    }
  //! 5 arguments
  template<class A,class B,class C,class D,class E> 
    ArrayException(A x,B y,C z,D a,E b): BaseExept()
    {
      cerr<<endl<<x<<" "<<y<<" "<<z<<" "<<a<<" "<<b<<endl<<endl;
    }
  //! 6 arguments
  template<class A,class B,class C,class D,class E,class F> 
    ArrayException(A x,B y,C z,D a,E b,F c): BaseExept()
    {
      cerr<<endl<<x<<" "<<y<<" "<<z<<" "<<a<<" "<<b<<
	" "<<c<<endl<<endl;
    }
  //! 7 arguments:
  template<class A,class B,class C,class D,class E,class F,class G> 
  ArrayException(A x,B y,C z,D a,E b,F c,G d)
    {
      cerr<<endl<<x<<" "<<y<<" "<<z<<" "<<a<<" "<<b<<
	" "<<c<<" "<<d<<endl<<endl;
    }
  //! 8 arguments:
  template<class A,class B,class C,class D,class E,class F,class G,class H> 
  ArrayException(A x,B y,C z,D a,E b,F c,G d,H e): BaseExept()
    {
      cerr<<endl<<x<<" "<<y<<" "<<z<<" "<<a<<" "<<b<<
	" "<<c<<" "<<d<<" "<<e<<endl<<endl;
    }
  //! 9 arguments:
  template<class A,class B,class C,class D,class E,class F,class G,class H,
	   class I> 
  ArrayException(A x,B y,C z,D a,E b,F c,G d,H e,I f): BaseExept()
    {
      cerr<<endl<<x<<" "<<y<<" "<<z<<" "<<a<<" "<<b<<
	" "<<c<<" "<<d<<" "<<e<<" "<<f<<endl<<endl;
    }
  //! 10 arguments:
  template<class A,class B,class C,class D,class E,class F,class G,class H,
	   class I,class J> 
  ArrayException(A x,B y,C z,D a,E b,F c,G d,H e,I f,J g): BaseExept()
    {
      cerr<<endl<<x<<" "<<y<<" "<<z<<" "<<a<<" "<<b<<
	" "<<c<<" "<<d<<" "<<e<<" "<<f<<" "<<g<<endl<<endl;
    }
};
}

#endif
