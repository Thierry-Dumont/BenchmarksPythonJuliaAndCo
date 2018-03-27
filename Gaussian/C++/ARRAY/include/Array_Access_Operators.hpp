#ifndef Array_Access_Operators__h
#define Array_Access_Operators__h
//! Access (ie. operator()) is delegated to the "engine".
//! \note we provide 2 versions ofteh operator: T& operator() and
//! T operator(), but this is notclear that compilers use the "T operator()"
//! version
//----- for 1d Arrays:
//! access operator
//! \param i1
inline T operator()(int i1) const 
{
#ifdef DEBUG
  if(!checkIndice(i1,0))
    throw ArrayException("Array() indice out of bounds",i1);
#endif
  return x[engine(i1)];
}
//! access operator
//! \param i1
inline T& operator()(int i1) 
{
#ifdef DEBUG
  if(!checkIndice(i1,0))
    throw ArrayException("Array() indice out of bounds",i1);
#endif
  return x[engine(i1)];
}

//----- for 2d Arrays:
//! access operator
//! \param i1
//! \param i2
inline T operator()(int i1,int i2) const 
{
#ifdef DEBUG
  if(!(checkIndice(i1,0) && checkIndice(i2,1) ))
     throw ArrayException("Array() indice out of bounds",i1,i2);
#endif
  return x[engine(i1,i2)];
}
//! access operator
//! \param i1
//! \param i2
inline T& operator()(int i1,int i2) 
{
#ifdef DEBUG
  if(!(checkIndice(i1,0) && checkIndice(i2,1) ))
     throw ArrayException("Array() indice out of bounds",i1,i2);
#endif
  return x[engine(i1,i2)];
}

//----- for 3d Arrays:
//! access operator
//! \param i1
//! \param i2
//! \param i3
inline T operator()(int i1,int i2,int i3) const 
{
#ifdef DEBUG
  if(!(checkIndice(i1,0) && checkIndice(i2,1) &&  checkIndice(i3,2) ))
    throw ArrayException("Array() indice out of bounds",i1,i2,i3);
#endif
  return x[engine(i1,i2,i3)];
}
//! access operator
//! \param i1
//! \param i2
//! \param i3
inline T& operator()(int i1,int i2,int i3)  
{
#ifdef DEBUG
  if(!(checkIndice(i1,0) && checkIndice(i2,1) &&  checkIndice(i3,2) ))
    throw ArrayException("Array() indice out of bounds",i1,i2,i3);
#endif
  return x[engine(i1,i2,i3)];
}
//----- for 4d Arrays:
//! access operator
//! \param i1
//! \param i2
//! \param i3
//! \param i4
inline T operator()(int i1,int i2,int i3,int i4) const 
{
#ifdef DEBUG
  if(!(checkIndice(i1,0) && checkIndice(i2,1) &&  checkIndice(i3,2)&&
       checkIndice(i4,3)))
      throw ArrayException("Array() indice out of bounds",i1,i2,i3,i4);
#endif
  return x[engine(i1,i2,i3,i4)];
}
//! access operator
//! \param i1
//! \param i2
//! \param i3
//! \param i4
inline T& operator()(int i1,int i2,int i3,int i4)  
{
#ifdef DEBUG
    if(!(checkIndice(i1,0) && checkIndice(i2,1) &&  checkIndice(i3,2)&&
	 checkIndice(i4,3)))
      throw ArrayException("Array() indice out of bounds",i1,i2,i3,i4);
#endif
  return x[engine(i1,i2,i3,i4)];
}
#endif
