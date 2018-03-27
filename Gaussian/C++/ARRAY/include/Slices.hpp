#ifndef Slices__h
#define Slices__h
#include <utility>
//! return a "natural" slice at line i (C like Array) or
//! at column i (Fortran like Array).
//! \note A "REFERENCE" is returned! thre is *no* copy of
//! the array of data; so the returned Array is flagged "not deletable"
//! and will actually not delete his datas when the destructor is called.
Array<T,n-1,Engine> slice_ref(int i)
{
#ifdef DEBUG
  pair<int,int> lims=engine.sliceLimits();
  if(i<lims.first||i>lims.second)
    throw ArrayException("Array::slice_ref, i=",i,"must be in [",lims.first,
			 lims.second,"[");
#endif
  Range R[n-1];
  pair<int,int> p=engine.sliceRangesIndexes();
  for(int j=p.first;j<p.second;j++) R[j-p.first]=engine.my_range(j);
  Array<T,n-1,Engine> A(R,false);
  A.x=x+engine.slice(i);
  return A;
}
#endif
