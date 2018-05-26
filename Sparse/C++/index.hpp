#pragma once
class index
{
  const int size;
  const int size2;
public:
  index(int _size):size(_size),size2(_size*_size)
  {
  }
  ~index()
  {
  }
  int operator()(int i,int j)
  {
    return i*size+j;
  }
  int operator()(int i,int j,int k)
  {
    return i*size2+j*size+k;
  }
};
