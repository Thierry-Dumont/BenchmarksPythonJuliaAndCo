#ifndef CEngine__h
#define CEngine__h
#include "ArrayException.hpp"
#include "Range.hpp"
#include <iostream>
#include <utility>
using namespace std;
////////////////////////////////////////////////////////////////////////
/// Manage a "C like" access.
/// store bounds...
///
/// \brief C-like indexing.
////////////////////////////////////////////////////////////////////////
namespace Arrays {
class CEngine
{
  int coeffs[6],Mins[6],Maxs[6],sub,AllocSize,usedDims;
public:
  // --------------------------------------------------------------
  //! constructor
  CEngine()
  {
    for(int i=0;i<6;i++) Mins[i]=0;
  }
  // --------------------------------------------------------------
  //! destructor
  ~CEngine(){
  }
  // --------------------------------------------------------------
  //! operator=
  //! \param CE
  inline void operator=(const CEngine& CE)
  {
    for(int i=0;i<6;i++) coeffs[i]=CE.coeffs[i];
    for(int i=0;i<6;i++) Mins[i]=CE.Mins[i];
    for(int i=0;i<6;i++) Maxs[i]=CE.Maxs[i];
    sub=CE.sub; AllocSize=CE.AllocSize; usedDims=CE.usedDims;
  }
  // --------------------------------------------------------------
  //! Initialize for 1d arrays.
  //! \param R Range
  inline void init(Range R) 
  {
    Mins[0]=R.begin; Maxs[0]=R.end;
    sub=R.begin; AllocSize=Maxs[0]-Mins[0];
    usedDims=1;
  }
  // --------------------------------------------------------------
  //! Initialize for 2d arrays.
  //! \param R0 (1rst indix)
  //! \param R1 size (2nd  indix)
  inline void init(Range R0,Range R1)
  {
    Mins[0]=R0.begin; Maxs[0]=R0.end;
    Mins[1]=R1.begin; Maxs[1]=R1.end;
    int n1=R1.end-R1.begin;
    coeffs[0]=n1;
    coeffs[1] = 1;
    sub= R0.begin*n1+R1.begin;
    AllocSize=(Maxs[0]-Mins[0])*(Maxs[1]-Mins[1]);
    usedDims=2;
  }
  // --------------------------------------------------------------
  //! Initialize or 3d arrays.
  //! \param R0 Range (1rst indix)
  //! \param R1 Range (2nd  indix)
  //! \param R2 Range (3rd  indix)
  inline void init(Range R0,Range R1,Range R2)
  {
    Mins[0]=R0.begin; Maxs[0]=R0.end;
    Mins[1]=R1.begin; Maxs[1]=R1.end;
    Mins[2]=R2.begin; Maxs[2]=R2.end;
    int n1=R1.end-R1.begin, n2=R2.end-R2.begin;
    coeffs[0]=n1*n2; coeffs[1]=n2; coeffs[2] = 1;
    sub=R0.begin*n1*n2+R1.begin*n2+R2.begin;
    AllocSize=(Maxs[0]-Mins[0])*(Maxs[1]-Mins[1])*(Maxs[2]-Mins[2]);
    usedDims=3;
  }
  // --------------------------------------------------------------
  //! Initialize or 4d arrays.
  //! \param R0 Range (1rst indix)
  //! \param R1 Range (2nd  indix)
  //! \param R2 Range (3rd  indix)
  //! \param R3 Range (3rd  indix)
  inline void init(Range R0,Range R1,Range R2,Range R3)
  {
    Mins[0]=R0.begin; Maxs[0]=R0.end;
    Mins[1]=R1.begin; Maxs[1]=R1.end;
    Mins[2]=R2.begin; Maxs[2]=R2.end;
    Mins[3]=R3.begin; Maxs[3]=R3.end;
    int n3=R3.end-R3.begin,n2=R2.end-R2.begin,n1=R1.end-R1.begin;
    coeffs[0]=n3*n2*n1; coeffs[1]=n3*n2; coeffs[2]=n3; coeffs[3] = 1;
    sub=R0.begin*n3*n2*n1+R1.begin*n3*n2+R2.begin*n3+R3.begin;
    AllocSize=(Maxs[0]-Mins[0])*(Maxs[1]-Mins[1])
      *(Maxs[2]-Mins[2])*(Maxs[3]-Mins[3]);
    usedDims=4;
  }
  //! initialization based on an array of Range's.
  //! \param R
  //! \param n size of R.
  inline void init(Range R[],int n)
  {
    switch(n){
    case 1: 
      {init(R[0]); break;}
    case 2:
      {init(R[0],R[1]); break;}
    case 3:
      {init(R[0],R[1],R[2]); break;}
    case 4:
      {init(R[0],R[1],R[2],R[3]); break;}
    default:
      throw ArrayException("Array::CEngine::init(Range R[],n)",n);
    }
  }
  // --------------------------------------------------------------
  //! access operator for 1d array
  //! \param i0
  inline int operator()(int i0) const {return i0-sub;}
  // --------------------------------------------------------------
  //! access operator for 2d array
  //! \param i0
  //! \param i1
  inline int operator()(int i0,int i1) const 
  {
    return i0*coeffs[0]+i1-sub;
  }
  // --------------------------------------------------------------
  //! access operator for 3d array
  //! \param i0
  //! \param i1
  //! \param i2
  inline int operator()(int i0,int i1,int i2) const 
  {
    return i0*coeffs[0] + i1*coeffs[1]+i2-sub;
  }
  // --------------------------------------------------------------
  //! access operator for 4d array
  //! \param i0
  //! \param i1
  //! \param i2
  //! \param i3
  inline int operator()(int i0,int i1,int i2,int i3) const 
  {
    return i0*coeffs[0] + i1*coeffs[1] + i2*coeffs[2] + i3 -sub;
  }
  //! return min value of indix i
  //! \param i
  inline int minIndix(int i) const {return Mins[i];}
  //! return min value for indice i.
  //! \param i
  inline int mins(int i) const {return Mins[i];}
  //! return max value for indice i.
  //! \param i
  inline int maxs(int i) const {return Maxs[i];}
  //! size of the C array we need to allocate.
  inline int size() const {return AllocSize;}
  //! return a copy of kth range
  inline Range my_range(int i) const
  {
    return Range( Mins[i],Maxs[i] );
  }
  //! where does the natural slice i starts?
  inline int slice(int i) const {return (i-Mins[0])*coeffs[0];}
  //! give the bounds of the "natural" slice  index.
  //! \note mainly for debugging.
  inline pair<int,int> sliceLimits() const
  {
    return make_pair(Mins[0],Maxs[0]-1);
  }
  //! where are the ranges for a slice?
  inline pair<int,int> sliceRangesIndexes() const
  {
    return make_pair(1,usedDims);
  }
};
}

#endif
