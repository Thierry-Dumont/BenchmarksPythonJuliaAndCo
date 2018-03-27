#ifndef FEngine__h
#define FEngine__h
#include "Range.hpp"
#include <iostream>
#include <utility>
using namespace std;
////////////////////////////////////////////////////////////////////////
/// Manage a "Fortran like" access.
/// store bounds...
///
/// \brief Fortran-like indexing.
////////////////////////////////////////////////////////////////////////
namespace Arrays {
class FEngine
{
  int coeffs[6],Mins[6],Maxs[6],sub,AllocSize,usedDims,sliceSize;
  
public:
  // -------------------------------------------------
  //! constructor
  FEngine()
  {
    for(int i=0;i<6;i++) Mins[i]=0;
  }
  // -------------------------------------------------
  //! destructor
  ~FEngine(){}
  // -------------------------------------------------
  //! operator=
  //! \param FE
  inline void operator=(const FEngine& FE)
  {
    for(int i=0;i<6;i++) coeffs[i]=FE.coeffs[i];
    for(int i=0;i<6;i++) Mins[i]=FE.Mins[i];
    for(int i=0;i<6;i++) Maxs[i]=FE.Maxs[i];
    sub=FE.sub; AllocSize=FE.AllocSize;
    usedDims=FE.usedDims;
    sliceSize=FE.sliceSize;
  }
  // -------------------------------------------------
  //! Initialize for 1d arrays.
  //! \param R Range
  inline void init(Range R) 
  {
    Mins[0]= R.one ? 1 : R.begin;
    Maxs[0]=R.end+1;
    sub=Mins[0] - 1;
    AllocSize=Maxs[0]-Mins[0]+1;
    usedDims=1;
  }
  // -------------------------------------------------
  //! Initialize for 2d arrays.
  //! \param R0 (1rst indix)
  //! \param R1 size (2nd  indix)
  inline void init(Range R0,Range R1)
  {
    Mins[0]=R0.one? 1 : R0.begin;
    Mins[1]=R1.one? 1 : R1.begin;
    Maxs[0]=R0.end+1; Maxs[1]=R1.end+1;
    int n0=Maxs[0]-Mins[0];
    coeffs[0]=1;
    coeffs[1] = n0;
    sub= (Mins[1]-1)*n0+(Mins[0]-1);
    AllocSize=(Maxs[0]-Mins[0])*(Maxs[1]-Mins[1]);
    usedDims=2;
    sliceSize=Maxs[0]-Mins[0];
  }
  // -------------------------------------------------
  //! Initialize or 3d arrays.
  //! \param R0 Range (1rst indix)
  //! \param R1 Range (2nd  indix)
  //! \param R2 Range (3rd  indix)
  inline void init(Range R0,Range R1,Range R2)
  {
    Mins[0]=R0.one? 1 : R0.begin;
    Mins[1]=R1.one? 1 : R1.begin;
    Mins[2]=R2.one? 1 : R2.begin;
    Maxs[0]=R0.end+1; Maxs[1]=R1.end+1;  Maxs[2]=R2.end+1;
    int n0=Maxs[0]-Mins[0],n1=Maxs[1]-Mins[1];
    coeffs[0] = 1; coeffs[1]=n0; coeffs[2]=n0*n1; 
    sub=(Mins[2]-1)*n0*n1+(Mins[1]-1)*n0+(Mins[0]-1);
    AllocSize=(Maxs[0]-Mins[0])*(Maxs[1]-Mins[1])
      *(Maxs[2]-Mins[2]);
    usedDims=3;
    sliceSize=(Maxs[0]-Mins[0])*(Maxs[1]-Mins[1]);
  }
  // -------------------------------------------------
  //! Initialize or 4d arrays.
  //! \param R0 Range (1rst indix)
  //! \param R1 Range (2nd  indix)
  //! \param R2 Range (3rd  indix)
  //! \param R3 Range (3rd  indix)
  inline void init(Range R0,Range R1,Range R2,Range R3)
  {
    Mins[0]=R0.one? 1 : R0.begin;
    Mins[1]=R1.one? 1 : R1.begin;
    Mins[2]=R2.one? 1 : R2.begin;
    Mins[3]=R3.one? 1 : R3.begin;
    Maxs[0]=R0.end+1; Maxs[1]=R1.end+1;  Maxs[2]=R2.end+1;
    Maxs[3]=R3.end+1; 
    int n0=Maxs[0]-Mins[0],n1=Maxs[1]-Mins[1];
    //int n2=R2.end-R2.begin;
    int n2=Maxs[2]-Mins[2];
    coeffs[0]=1; coeffs[1]=n0; coeffs[2]=n0*n1;
    coeffs[3] = n0*n1*n2;
    sub=(Mins[3]-1)*n0*n1*n2+(Mins[2]-1)*n0*n1+(Mins[1]-1)*n0+(Mins[0]-1);
    AllocSize=(Maxs[0]-Mins[0])*(Maxs[1]-Mins[1])
      *(Maxs[2]-Mins[2])*(Maxs[3]-Mins[3]);
    usedDims=4;
    sliceSize=(Maxs[0]-Mins[0])*(Maxs[1]-Mins[1])*(Maxs[2]-Mins[2]);
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
      throw ArrayException("Array::FEngine::init(Range R[],n)",n);
    }
  }
  // -------------------------------------------------
  //! access operator for 1d array
  //! \param i1
  inline int operator()(int i1) const {return i1-sub-1;}
  // -------------------------------------------------
  //! access operator for 2d array
  //! \param i0
  //! \param i1
  inline int operator()(int i0,int i1) const 
  {
    return i0+(i1-1)*coeffs[1]-sub-1;
  }
  // -------------------------------------------------
  //! access operator for 3d array
  //! \param i0
  //! \param i1
  //! \param i2
  inline int operator()(int i0,int i1,int i2) const 
  {
    return i0+(i1-1)*coeffs[1]+(i2-1)*coeffs[2]-sub-1;
  }
  // -------------------------------------------------
  //! access operator for 4d array
  //! \param i0
  //! \param i1
  //! \param i2
  //! \param i3
  inline int operator()(int i0,int i1,int i2,int i3) const 
  {
    return i0+(i1-1)*coeffs[1]+(i2-1)*coeffs[2]+(i3-1)*coeffs[3]-sub-1;
  }
  // -------------------------------------------------
  //! return min value of indix i
  //! \param i
  inline int minIndix(int i) const {return Mins[i];}
  // -------------------------------------------------
  //! return min value for indice i.
  //! \param i
  inline int mins(int i) const {return Mins[i];}
  // -------------------------------------------------
  //! return max value for indice i.
  //! \param i
  //! \note bound is included!
  inline int maxs(int i) const {return Maxs[i];}
  // -------------------------------------------------
  //! size of the C array we need to allocate.
  inline int size() const {return AllocSize;}
  //! return a copy of kth range
  inline Range my_range(int i) const
  {
    return Range( Mins[i],Maxs[i]-1);
  }
  //! return coeffs[0], which is the size of a "natural" sub array.
  //inline int NaturalSubArraySize() const {return coeffs[usedDims-1];}
  inline int slice(int i) const 
  {
    if(usedDims==1)
      throw ArrayException("Array: no slice for 1d Array's!");
    else
      return (i-Mins[usedDims-1])*sliceSize;
  }
  //! give the bounds of the "natural" slice  index.
  //! \note mainly for debugging.
  inline pair<int,int> sliceLimits() const
  {
    return make_pair(Mins[usedDims-1],Maxs[usedDims-1]);
  }
  //! where are the ranges for a slice?
  inline pair<int,int> sliceRangesIndexes() const
  {
    return make_pair(0,usedDims-1);
  }
};
}
#endif
