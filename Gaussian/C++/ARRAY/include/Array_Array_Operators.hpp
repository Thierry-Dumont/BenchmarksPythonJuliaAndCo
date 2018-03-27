#ifndef Array_Array_Operators__h
#define Array_Array_Operators__h
//! Add an array with the same pattern
//! \param X
//! \note if there is no conversion from Q to T => error at compile time.
template<class Q> inline void operator+=(Array<Q,n,Engine>& X)
{
#ifdef DEBUG
  if(!checkSamePattern(X))
    throw ArrayException("Array::operator+= : patterns differ");
#endif
  const int temp=xsize;
  if( temp!=X.size() )
    throw ArrayException("Array += : note the same size");
  Q* Xx=X.data();
#ifdef ICC
#pragma ivdep
#endif
  for(int i=0;i<temp;i++) x[i]+=Xx[i];
}
//! Substract an array with the same pattern
//! \param X
//! \note if there is no conversion from Q to T => error at compile time.
template<class Q> inline void operator-=(Array<Q,n,Engine>& X)
{
#ifdef DEBUG
  if(!checkSamePattern(X))
    throw ArrayException("Array::operator-= : patterns differ");
#endif
  if( xsize!=X.size() )
    throw ArrayException("Array -= : note the same size");
  Q* Xx=X.data();
  for(int i=0;i<xsize;i++) x[i]-=Xx[i];
}
#endif
