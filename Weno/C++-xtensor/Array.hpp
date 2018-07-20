#pragma once
template<typename T,int n> class Array
{
  T x[n];
public:
  T& operator[](int i) {return x[i];}
};
