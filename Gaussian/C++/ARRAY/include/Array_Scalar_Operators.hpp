#ifndef Array_Scalar_Operators__h
#define Array_Scalar_Operators__h
//! Make all the components equal the same value
//! \param value
inline void operator=(T value)
{
  for(int i=0;i<xsize;i++) 
    x[i]=value;
}
//! Add the same value to all components
//! \param value
inline void operator+=(T value)
{
  for(int i=0;i<xsize;i++) x[i]+=value;
}
//! Substract the same value to all components
//! \param value
inline void operator-=(T value)
{
  for(int i=0;i<xsize;i++) x[i]-=value;
}
//! Multiply all components by the same value
//! \param value
inline void operator*=(T value)
{
  for(int i=0;i<xsize;i++) x[i]*=value;
}
//! Divide all components by the same value
//! \param value
inline void operator/=(T value)
{
  for(int i=0;i<xsize;i++) x[i]/=value;
}
#endif
