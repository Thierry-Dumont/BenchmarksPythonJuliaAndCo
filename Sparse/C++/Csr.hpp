#pragma once
#include "PreSparse.hpp"
#include <memory>
#include <tuple>
#include <exception>
#include <iostream>
class Csr
{
  std::unique_ptr<double[]> V;
  std::unique_ptr<int[]> ia,ja;
  int nlig,nc;
public:
  Csr(const PreSparse& P)
  {
    int ncol;
    std::tie(nlig,ncol,nc)=P.sizes();
    if(nlig!=ncol)
      throw std::runtime_error("Csr: not a square matrix.");
    ia=std::make_unique<int[]>(nc);
    ja=std::make_unique<int[]>(nlig+1);
    V=std::make_unique<double[]>(nc);
    //
    ja[0]=1;
    int pia=0,curja=1;
    auto Iend=P.cend();
    for(auto I=P.cbegin();I!=Iend;I++)
      {
	int cc=I->first.first;
	if(cc!=curja)
	  {
	    curja=cc;
	    ja[curja-1]=pia+1;
	  }
	ia[pia]=I->first.second;
	V[pia++]=I->second;
      }
    ja[curja]=pia+1;
    //std::cout<<I->first.first<<" "<<I->first.second<<std::endl;
  
  }
  ~Csr()
  {
  }
  void prod(std::unique_ptr<double[]>& In, std::unique_ptr<double[]>& Out)
  {
    for(int i=0;i<nlig;i++)
      {
	double t=0.0;
	for(int j=ia[i];j<ia[i+1]-1;j++)
        {
#ifdef DEBUG
          if(j<0||ja[j]<0)
            throw  throw std::runtime_error("Csr: bad indice.");
#endif
          t+=V[j]*In[ja[j]];
        }
      Out[i]=t;
    }

  }
};
