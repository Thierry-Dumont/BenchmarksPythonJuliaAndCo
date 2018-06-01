#include "PreSparse.hpp"
#include "index.hpp"
#include <math.h>
#include <iostream>
template<int dim> void PreLapl(PreSparse& M,int size)
{
  double h=1./(size-1), h2=h*h, cd=-4/h2,hd=1./h2;
  int n2=size*size;
  index I(size);

  for(int i=0;i<size;i++)
    for(int j=0;j<size;j++)
      {
	auto l=I(i,j);
	M(l,l)=cd;
      	for(int i1=-1;i1<=1;i1+=2)
	  for(int j1=-1;j1<=1;j1+=2)
	    {
	      auto l1=I(i+i1,j+j1);
	      if(l1>=0 && l1<n2)
		M(l,l1)=hd;
	    }
      }
}
template<> void PreLapl<3>(PreSparse& M,int size)
{
  double h=1./(size-1), h2=h*h, cd=-6/h2,hd=1./h2;
  int n3=pow(size,3);
  index I(size);

  for(int i=1;i<=size;i++)
    for(int j=1;j<=size;j++)
      for(int k=1;k<=size;k++)
	{
	  auto l=I(i,j,k);
	  M(l,l)= cd;
	  for(int i1=-1;i1<=1;i1+=2)
	    for(int j1=-1;j1<=1;j1+=2)
	      for(int k1=-1;k1<=1;k1+=2)
	  	{
	  	  int l1=I(i+i1,j+j1,k+k1);
	  	  if(l1>=0 && l1<n3)
	  	    M(l,l1)=hd;
	  	}
	}
 
}
