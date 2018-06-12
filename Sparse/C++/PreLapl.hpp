#include "PreSparse.hpp"
#include <math.h>
#include <iostream>
#include <memory>
#include <tuple>

template<int dim> pair<int,int> PreLapl(std::shared_ptr<int[]>& row,
			       std::shared_ptr<int[]>& col,
			       std::shared_ptr<double[]>& v
			       int size)

{
  double h=1./(size-1), h2=h*h, cd=-4/h2,hd=1./h2;
  int order=pow(size,2);
  int nc= 5*pow(size-2,2)+ 16*(size-2)+ 12;

  auto I=[size](int i,int j){return i*size+j;};

  int count=0;
  for(int i=0;i<size;i++)
    for(int j=0;j<size;j++)
      {
  	auto l=I(i,j);
  	row[count]=l;
  	col[count]=l;
  	v[count++]=cd;
      	for(int i1=-1;i1<=1;i1+=2)
  	  {
  	    if(i+i1>=0 && i+i1<size)
  	      {
  		row[count]=l;
  		col[count]=I(i+i1,j);
  		v[count++]=hd;
  	      }
  	    if(j+i1>=0 && j+i1<size)
  	      {
  		row[count]=l;
  		col[count]=I(i,j+i1);
  		v[count++]=hd;
  	      }
  	  }
      }
  return make_pair(order,nc);
  
}
template<int dim> void PreLapl(PreSparse& M,int size)
{
  double h=1./(size-1), h2=h*h, cd=-4/h2,hd=1./h2;
  auto I=[size](int i,int j){return i*size+j;};

  for(int i=0;i<size;i++)
    for(int j=0;j<size;j++)
      {
	auto l=I(i,j);
	M(l,l)=cd;
      	for(int i1=-1;i1<=1;i1+=2)
	  {
	    if(i+i1>=0 && i+i1<size)
	      M(l,I(i+i1,j))=hd;
	    if(j+i1>=0 && j+i1<size)
	      M(l,I(i,j+i1))=hd;

	  }
      }
}
template<> void PreLapl<3>(PreSparse& M,int size)
{
  double h=1./(size-1), h2=h*h, cd=-6/h2,hd=1./h2;
  const int size2=size*size;
  auto I=[size,size2](int i,int j,int k){return i*size2+j*size+k;};
  
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
		  if(i+i1>=0 && i+i1<size)
		    M(l,I(i+i1,j,k))=hd;
		  if(j+i1>=0 && j+i1<size)
		    M(l,I(i,j+i1,k))=hd;
		  if(k+i1>=0 && k+i1<size)
		    M(l,I(i,j,k+i1))=hd;

	  	}
	}
 
}
