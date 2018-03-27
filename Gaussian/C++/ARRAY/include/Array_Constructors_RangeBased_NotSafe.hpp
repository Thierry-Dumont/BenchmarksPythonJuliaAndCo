#ifndef Array_Constructors_RangeBased_NotSafe__h
#define Array_Constructors_RangeBased_NotSafe__h
public:
//////////////////////////////////////////////////////////////////////////////
/// With these constructors, an Array "adopts" _x, a vector of T.
/// No check is done about the real size of the adopted vector, and thus
/// this is not safe.
/// Note that the Array "x" vector is not deletable.
//////////////////////////////////////////////////////////////////////////////
//! constructor for 1d Array, based on a range.
//! \param R0 range.
//! \param _x the adopted vector.
//! \note not a safe constructor!
Array(Range R0,T *_x)
{
  if(n!=1) 
    throw ArrayException("Array constructor incompatible with dimension=",n);
  engine.init(R0);
  xsize=engine.size();
  x=_x;
  deletable=false;
}
//! constructor for 2d Array, based on ranges.
//! \param R0 range.
//! \param R1 range
//! \param _x the adopted vector.
//! \note not a safe constructor!
Array(Range R0,Range R1,T *_x)
{
  if(n!=2) 
    throw ArrayException("Array constructor incompatible with dimension=",n);
  engine.init(R0,R1);
  xsize=engine.size();
  x=_x;
  deletable=false;
}
//! constructor for 3d Array, based on ranges.
//! \param R0 range.
//! \param R1 range
//! \param R2 range
//! \param _x the adopted vector.
//! \note not a safe constructor!
Array(Range R0,Range R1,Range R2,T *_x)
{
  if(n!=3) 
    throw ArrayException("Array constructor incompatible with dimension=",n);
  xsize=(R0.end-R0.begin)*(R1.end-R1.begin)*(R2.end-R2.begin);
  engine.init(R0,R1,R2);
  xsize=engine.size();
  x=_x;
  deletable=false;
}
//! constructor for 4d Array, based on ranges.
//! \param R0 range.
//! \param R1 range
//! \param R2 range
//! \param R3 range
//! \param _x the adopted vector.
//! \note not a safe constructor!
Array(Range R0,Range R1,Range R2,Range R3,T *_x)
{
  if(n!=4) 
    throw ArrayException("Array constructor incompatible with dimension=",n);
  xsize=(R0.end-R0.begin)*(R1.end-R1.begin)*(R2.end-R2.begin)*
    (R3.end-R3.begin);
  engine.init(R0,R1,R2,R3);
  xsize=engine.size();
  x=_x;
  deletable=false;
}

#endif
