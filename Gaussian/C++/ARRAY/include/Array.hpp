#ifndef Array__h
#define Array__h
#include "Range.hpp"
#include "CEngine.hpp"
#include "FEngine.hpp"
#include "ArrayException.hpp"
#include "Array_iterators.hpp"
#include "MacroRestrict.hpp"
////////////////////////////////////////////////////////////////////////
/// Array class.
/// Template parameters:
///   T  : type of data stored.
///   n  : dimension (ie. number of indices).
///   Engine:  indexation; default is a C-like indexation.
///
/// \brief Array class.
///////////////////////////////////////////////////////////////////////
using namespace std;
namespace Arrays{ 
template<class T,int n,class Engine=CEngine> class Array
{
  template<class TT,int nn,class EEngine> friend 
  ostream& operator<< (ostream &out,const  Array<TT,nn,EEngine> &A);
  friend class Array<T,n+1,Engine>;
  //-----all fields are here-----
#ifdef CLANG
  T *x;
#else
  T * Restrict x; //!< container
#endif
  int xsize; //!< size of the container.
  Engine engine;
  bool deletable;
  //----------------------------
  //! check that Array A has the same pattern as us.
  //! \param A
  template<class Q> inline bool checkSamePattern(const Array<Q,n,Engine>& A) 
    const
  {
    bool ret=true;
    if(xsize!=A.size())
      ret=false;
    else
      for(int i=0;i<n;i++)
	{
	  ret=mins(i)==A.mins(i) && maxs(i)==A.maxs(i);
	  if(!ret) break;
	}
    return ret;
  }
  //! check for indice correctness.
  //! \param i the indice we check
  //! \param c the component.
  inline bool checkIndice(int i,int c) const
  {
    return i>=mins(c) && i<maxs(c);
  }
  //! set the deletable flag
  //! this is supposed to be used only by friends.
  //! \param value true or false.
  inline void set_deletable(bool value) {deletable=value;}

 
public:
  typedef ArrayIterator<T> iterator;
  typedef T value_type;
#include "Array_Constructors_RangeBased.hpp"
#include "Array_Constructors_RangeBased_NotSafe.hpp"
  //! this constructor build an array which *cannot* be used.
  //! you must call an "resize" method after.
  //! \note this avoid creation of pointer in classes.
  Array()
  {
    x=NULL;
    xsize=0;
    deletable=false;
  }
#include "Array_resize.hpp"
  //! Copy Constructor
  //!\param X we build the Array from X.
  //! \note we create, but we DO NOT copy datas.
  Array(const Array<T,n,Engine>& X)
  {
    xsize=X.xsize; 
    engine=X.engine;
    x=new T[xsize];
    deletable=true;
  }
  //! Operator=
  //! \param X
  //! \note if there is no implicit conversion from Q to T, an error
  //! will be detected at compile time.
  template<class Q> inline void operator=(Array<Q,n,Engine>& X)
  {
#ifdef DEBUG
    if(!checkSamePattern(X))
      throw ArrayException("Array::operator= : patterns differ");
#endif
    if( xsize!=X.size() )
      throw ArrayException("Array = : not the same sizes",xsize,X.size());
    Q* Xx=X.data(); // => X cannot be declared const!
    for(int i=0;i<xsize;i++) x[i]=Xx[i];
    deletable=true;
  }
  //! return a "reference" to an Array A: this means that "this" Array
  //! and A share all data.
  //! \param A
  //! \note *this must not be marked deletable: *this will share datas
  //! with A.
  void reference(Array<T,n,Engine>& A)
  {
#ifdef DEBUG
    if(deletable)
      throw ArrayException("Array::reference: Array is marked deletable");
#endif
    x=A.x; xsize=A.xsize; engine=A.engine;
    deletable=false;
  } 
  //! destructor.
  ~Array()
  {
    if (x != NULL && deletable) delete[] x;
  }
#include "Array_Access_Operators.hpp"
#include "Array_Array_Operators.hpp"
#include "Array_Scalar_Operators.hpp"
  
  //! return a pointer to the container.
  inline T* data() {return x;}

  //! return the size of the Array (size of x!)
  inline int size() const {return xsize;}
  //! return min value for indice i.
  //! \param i
  inline int mins(int i) const {return engine.mins(i);}
  //! return max value for indice i.
  //! \param i
  inline int maxs(int i) const {return engine.maxs(i);}

  //! return an iterator pointing to the begining.
  inline iterator begin() {return iterator(x);}
  //! return an iterator pointing to the end.
  inline iterator end()  {return iterator(x+xsize);}

  //! cycle: permutation of the data of two Arrays.
  //! \param A
  //! \note arrays A and "this" must have the same pattern: this is checked *only*
  //! in debug mode.
  void cycle(Array<T,n,Engine>& A)
  {
#ifdef DEBUG
    if(!checkSamePattern(A))
      throw ArrayException("Array::cycle: patterns differ");
#endif
    if(xsize!=A.xsize)
      throw ArrayException("Array::cycle: sizes differ");

    T *t= x; x=A.x; A.x=t;
  }
  //! cycle: circular permutation of the data of 3 Arrays.
  //! \param A1
  //! \param A2
  //! \note array A1, A2 and "this" must have the same pattern: this is checked *only*
  //! in debug mode.
  void cycle(Array<T,n,Engine>& A1,Array<T,n,Engine>& A2)
  {
#ifdef DEBUG
    if(!checkSamePattern(A1) || !checkSamePattern(A2))
      throw ArrayException("Array::cycle: patterns differ");
#endif
    if(xsize!=A1.xsize || xsize!=A2.xsize)
      throw ArrayException("Array::cycle: sizes differ");

    T *t= x; x=A1.x; A1.x= A2.x; A2.x=t;
  }
  //! position: given an iterator, compute the position it points to.
  inline int position(iterator& it) const
  {
    return it.Tpointer()-x;
  }
#include "Slices.hpp"
#include "ApplyFonc.hpp"
};
}

#endif
