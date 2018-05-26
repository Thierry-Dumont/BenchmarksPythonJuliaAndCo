#include "PreSparse.hpp"
#include "index.hpp"
template<int dim> void PreLapl(PreSparse& M,int size)
{
  double h=1./(size-1), h2=h*h, cd=-4/h2,hd=1./h2;
  index I(size);
  for(int i=0;i<size;i++)
    for(int j=0;j<size;j++)
      M(I(i,j),I(i,j))=cd;
  for(int i=1;i<size-1;i++)
    for(int j=1;j<size-1;j++)
      {
	M(I(i,j),I(i-1,j))=hd;
	M(I(i,j),I(i+1,j))=hd;
	M(I(i,j),I(i,j-1))=hd;
	M(I(i,j),I(i,j+1))=hd;
      }
}
template<> void PreLapl<3>(PreSparse& M,int size)
{
  double h=1./(size-1), h2=h*h, cd=-6/h2,hd=1./h2;
  index I(size);
  for(int i=0;i<size;i++)
    for(int j=0;j<size;j++)
      for(int k=0;k<size;k++)
	M(I(i,j,k),I(i,j,k))=cd;
  for(int i=1;i<size-1;i++)
    for(int j=1;j<size-1;j++)
      for(int k=1;k<size-1;k++)
	{
	  M(I(i,j,k),I(i-1,j,k))=hd;
	  M(I(i,j,k),I(i+1,j,k))=hd;
	  M(I(i,j,k),I(i,j-1,k))=hd;
	  M(I(i,j,k),I(i,j+1,k))=hd;
	  M(I(i,j,k),I(i,j,k-1))=hd;
	  M(I(i,j,k),I(i,j,k+1))=hd;
	}
}
