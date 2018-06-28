#pragma once
#include <utility>
#include <iostream>
#include <memory>
#include <cmath>
//#include "/usr/local/IACA/iaca-lin64/include/iacaMarks.h"
template<class Flux> class Weno
{
  int size;
  double h1;
  
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
  std::unique_ptr<double[]> reconstructed,InC,work;
  double right[3],left[3],alphaleft[3],alpharight[3],beta[3];
  Flux F;

 
public:
  Weno(){}
  Weno(int _size,double _L,double FluxParams[]): size(_size),h1(-1./(_L/size))
  {
    F=Flux(FluxParams);
    // 
    reconstructed=std::make_unique<double[]>(2*size+8);
    InC=std::make_unique<double[]>(size+4);
    work=std::make_unique<double[]>(size+2);
    
  }
  Weno& operator=(Weno<Flux>&& W)
  {
    size=W.size;
    F=std::move(W.F);
    
    reconstructed=std::move(W.reconstructed);
    InC=std::move(W.InC);
    work=std::move(W.work);
    
    h1=W.h1;
    return *this;
  }
  ~Weno()
  {
  }
  void operator()(std::unique_ptr<double []>& In,
		  std::unique_ptr<double []>& Out)
  {
    // build an extended array with phantom cells to deal with periodicity:
    InC[0]=In[size-2];
    InC[1]=In[size-1];
    for(int i=0;i<size;i++)
      InC[i+2]=In[i];
    InC[size+2]=In[0];
    InC[size+3]=In[1];
    // precompute for regularity coefficients
    for(int vol=0;vol<size+2;vol++)
      work[vol]= b0*pow(InC[vol]-2.0*InC[vol+1]+InC[vol+2],2);
    
    // lets's start computation:
    for(int vol=2;vol<size+2;vol++)
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
	beta[0]= work[vol]+  b1*pow(3.*InC[vol]-4.*InC[vol+1]+InC[vol+2],2);
        
        beta[1]= work[vol-1]+b1*pow(InC[vol-1]-InC[vol+1],2);
        
	beta[2]= work[vol-2]+b1*pow(InC[vol-2]-4.*InC[vol-1]+3*InC[vol],2);


        double sleft=0.0;
        double sright=0.0;
	//IACA_START	 	
	for(int r=0;r<3;r++)
	  {
            double s=1./pow(epsilon+beta[r],2);
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
    for(int vol=1;vol<size+1;vol++)
      work[vol]=F(reconstructed[2*vol+3], reconstructed[2*vol+4]);
    work[0]= work[size];

    
    //result:

    for(int vol=0;vol<size;vol++)
      Out[vol]=h1*(work[vol+1]-work[vol]);

  }
  
};
