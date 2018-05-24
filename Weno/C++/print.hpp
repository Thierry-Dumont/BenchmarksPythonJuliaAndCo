#pragma once
#include <iostream>
#include <string>
#include <memory>
using namespace std;
template<class T> void print(T& X,int size,string s="")
{
  cout<<s<<endl;
  for(int i=0;i<size;i++)
    {
      if(i%4 ==0 && i>0) cout<<endl;
      cout<<X[i]<<" ";
    }
  cout<<endl;
}
