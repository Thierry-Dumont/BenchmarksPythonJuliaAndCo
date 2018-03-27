#ifndef Array_Constructors_RangeBased__h
#define Array_Constructors_RangeBased__h
private:
//! constructor based on an array of Range
//! \note x array is created or not, depending on _deletable.
//! \note this is for internal use, to build references to sub-arrays.
//! \param R
//! \param _deletable
Array(Range R[],bool _deletable=true)
{
  engine.init(R,n);
  xsize=engine.size();
  deletable=_deletable;
  if(_deletable)
    x=new T[xsize];
}
public:
//! constructor for 1d Array, based on a range.
//! \param R0 range.
Array(Range R0)
{
  if(n!=1) 
    throw ArrayException("Array constructor incompatible with dimension=",n);
  engine.init(R0);
  xsize=engine.size();
  x=new T[xsize];
}
//! constructor for 2d Array, based on ranges.
//! \param R0 range.
//! \param R1 range
Array(Range R0,Range R1)
{
  if(n!=2) 
    throw ArrayException("Array constructor incompatible with dimension=",n);
  engine.init(R0,R1);
  xsize=engine.size();
  x=new T[xsize];
}
//! constructor for 3d Array, based on ranges.
//! \param R0 range.
//! \param R1 range
//! \param R2 range
Array(Range R0,Range R1,Range R2)
{
  if(n!=3) 
    throw ArrayException("Array constructor incompatible with dimension=",n);
  xsize=(R0.end-R0.begin)*(R1.end-R1.begin)*(R2.end-R2.begin);
  engine.init(R0,R1,R2);
  xsize=engine.size();
  x=new T[xsize];
}
//! constructor for 4d Array, based on ranges.
//! \param R0 range.
//! \param R1 range
//! \param R2 range
//! \param R3 range
Array(Range R0,Range R1,Range R2,Range R3)
{
  if(n!=4) 
    throw ArrayException("Array constructor incompatible with dimension=",n);
  xsize=(R0.end-R0.begin)*(R1.end-R1.begin)*(R2.end-R2.begin)*
    (R3.end-R3.begin);
  engine.init(R0,R1,R2,R3);
  xsize=engine.size();
  x=new T[xsize];
}

#endif
