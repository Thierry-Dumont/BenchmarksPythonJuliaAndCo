#ifndef ApplyFonc__h
#define ApplyFonc__h
//! apply a function F (of one variable) to the whole Array.
template<T F(T)> void applyFonc()
{
  for(int i=0;i<xsize;i++)
    x[i]=F(x[i]);
}
#endif
