#pragma once
#include "xtensor/xtensor.hpp"
#include <utility>
#include <iostream>
#include <memory>
#include <cmath>
//#include "/usr/local/IACA/iaca-lin64/include/iacaMarks.h"
template<class Flux,std::size_t size> class Weno
{

  double h1;
  const int sizei=size;
  // Weno-3 coefficients:  --------------------
  const double c[4][3]={
    {11./6.,-7./6.,1./3.},
    {1./3.,5./6.,-1./6.},
    {-1./6.,5./6.,1./3.},
    {1./3.,-7/6.,11./6.}
  };
  const double dright[3]={3./10.,3./5.,1./10.};
  const double dleft[3]={1./10.,3./5.,3.10};
  const double b0=13./12.,b1=1./4.;
  const double epsilon=1.e-6;
  //-------------------------------------------

  xt::xtensor_fixed<double, xt::xshape<2*size+8>> reconstructed;
  xt::xtensor_fixed<double, xt::xshape<size+4>> InC;
  xt::xtensor_fixed<double, xt::xshape<size+2>> work;
  
  double right[3],left[3],alphaleft[3],alpharight[3],beta[3];
  Flux F;

 
public:
  Weno(){}
  Weno(double _L,double FluxParams[]): h1(-1./(_L/size))
  {
    F=Flux(FluxParams);
  }
  Weno& operator=(Weno<Flux,size>&& W)
  {

    F=std::move(W.F);
    h1=W.h1;
    return *this;
  }
  ~Weno()
  {
  }
  void operator()(xt::xtensor_fixed<double, xt::xshape<size>>& In,
		  xt::xtensor_fixed<double, xt::xshape<size>>& Out)
  {
    // build an extended Array with phantom cells to deal with periodicity:
    InC[0]=In[size-2];
    InC[1]=In[size-1];
    for(int i=0;i<sizei;i++)
      InC[i+2]=In[i];
    InC[size+2]=In[0];
    InC[size+3]=In[1];
    // precompute for regularity coefficients
    for(int vol=0;vol<sizei+2;vol++)
      work[vol]= b0*::std::pow(InC[vol]-2.0*InC[vol+1]+InC[vol+2],2);
    
    // lets's start computation:
    for(int vol=2;vol<sizei+2;vol++)
      {
	
	for(int r=0;r<3;r++)
	  {
	    left[r]=0.0;
	    right[r]=0.0;
   
	    for(int j=0;j<3;j++)
	      {
		left[r]+=c[r][j]*InC[vol-r+j];
		right[r]+=c[r+1][j]*InC[vol-r+j];
	      }

	  }

	// regularity coefficients:
	beta[0]= work[vol]+  b1*::std::pow(3.*InC[vol]-4.*InC[vol+1]+InC[vol+2],2);
        
        beta[1]= work[vol-1]+b1*::std::pow(InC[vol-1]-InC[vol+1],2);
        
	beta[2]= work[vol-2]+b1*::std::pow(InC[vol-2]-4.*InC[vol-1]+3*InC[vol],2);


        double sleft=0.0;
        double sright=0.0;
	//IACA_START	 	
	for(int r=0;r<3;r++)
	  {
            double s=1./::std::pow(epsilon+beta[r],2);
	    //double s=1./((epsilon+beta[r])*(epsilon+beta[r]));
	    alpharight[r]=dright[r]*s;
            alphaleft[r]=dleft[r]*s;
            sright+=alpharight[r];
            sleft+=alphaleft[r];
	  }
	//IACA_END 
	double recleft=0.0;
	for(int i=0;i<3;i++)
	  recleft += alphaleft[i]*left[i];
	double recright=0.0;
	for(int i=0;i<3;i++)
	  recright += alpharight[i]*right[i];
	reconstructed[2*vol]  = recleft/sleft;
	reconstructed[2*vol+1]= recright/sright;
      }
    for(int i=4;i<8;i++)
      reconstructed[2*size+i]=reconstructed[i];
 
    // Numerical flux at boundaries of volumes:
    for(int vol=1;vol<sizei+1;vol++)
      work[vol]=F(reconstructed[2*vol+3], reconstructed[2*vol+4]);
    work[0]= work[size];

    
    //result:

    for(int vol=0;vol<sizei;vol++)
      Out[vol]=h1*(work[vol+1]-work[vol]);

  }
  
};
