#ifndef Array_Output__h
#define Array_Output__h
#include "Array.hpp"
#include "ArrayException.hpp"
#include <iostream>
namespace  Arrays{ 
  ////////////////////////////////////////////////////////////////////
  /// Overload operator <<.
  ///
  /// This is restricted to Arrays with n<=2 indices!
  ///////////////////////////////////////////////////////////////////
  //! \param out the stream.
  //! \param A the array.
  template<class T,int n,class Engine>  std::ostream& 
             operator<<(std::ostream& out,const Array<T,n,Engine> &A)
  {
    if(n>2)
      out<<endl<<"Arrays::Array, with n= "<<n<<
	" no operator<< for n>2"<<endl;
    else
      if(n==1)
	for(int i=A.mins(0);i<A.maxs(0);i++)
	  out<<A(i)<<" ";
      else //n==2.
	for(int i=A.mins(0);i<A.maxs(0);i++)
	  {
	    for(int j=A.mins(1);j<A.maxs(1);j++)
	      out<<A(i,j)<<" ";
	    out<<endl<<endl;;
	  }

    return out;
  }
};
#endif
