#ifndef Array_resize__h
#define Array_resize__h
private:
//! Really resize.
//! \param  the_size
inline void really_resize(int the_size)
{
  if(x!=NULL) delete[] x;
  x=new T[xsize];
  deletable=true;
}
public:
//! Resize a 1d Array using a Range.
//! \param  R0
void resize(Range R0)
{
  if(n!=1) 
    throw ArrayException("Array: resize incompatible with dimension=",n);
  engine.init(R0);
  xsize=engine.size();
  really_resize(xsize);

}
//! Resize a 2d Array using Ranges.
//! \param  R0
//! \param R1
void resize(Range R0,Range R1)
{
  if(n!=2) 
    throw ArrayException("Array: resize incompatible with dimension=",n);
  engine.init(R0,R1);
  xsize=engine.size();
  really_resize(xsize);
}
//! Resize a 3d Array using Ranges.
//! \param  R0
//! \param  R1
//! \param  R2
void resize(Range R0,Range R1,Range R2)
{
  if(n!=3) 
    throw ArrayException("Array: resize incompatible with dimension=",n);
  engine.init(R0,R1,R2);
  xsize=engine.size();
  really_resize(xsize);
}
//! Resize a 3d Array using Ranges.
//! \param  R0
//! \param  R1
//! \param  R2
//! \param  R3
void resize(Range R0,Range R1,Range R2,Range R3)
{
  if(n!=4) 
    throw ArrayException("Array: resize incompatible with dimension=",n);
  engine.init(R0,R1,R2,R3);
  xsize=engine.size();
  really_resize(xsize);
}
#endif
