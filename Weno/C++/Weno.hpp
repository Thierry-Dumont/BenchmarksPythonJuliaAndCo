#pragma once
#include <utility>
#include <iostream>
#include <memory>
#include <cmath>
template<class Flux> class Weno
{
  int size;
  double h1;
  double *reconstructed, *numflux;
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
    
  Flux F;

  inline int index(int i)
  {
    return i>=0 ? i%size: size-i;
  }
public:
  Weno(){}
  Weno(int _size,double _L,double FluxParams[]): size(_size),h1(-1./(_L/size))
  {
    F=Flux(FluxParams);
    //
    reconstructed=new double[2*size];
    numflux= new double[size];
  }
  Weno& operator=(Weno<Flux>&& W)
  {
    size=W.size;
    F=std::move(W.F);
    reconstructed=W.reconstructed;
    numflux=W.numflux;
    h1=W.h1;
    W.reconstructed=NULL; W.numflux=NULL;
    
    return *this;
  }
  ~Weno()
  {
    delete[] reconstructed;
    delete[] numflux;
   
  }
  void operator()(std::unique_ptr<double []>& In,
		  std::unique_ptr<double []>& Out)
  {
    double right[3],left[3];
    for(int vol=0;vol<size;vol++)
      {
	//reconstructions right & left:
	for(int r=0;r<3;r++)
	  {
	    right[r]=0.; left[r]=0;
	    for(int j=0;j<3;j++)
	      {
		right[r]+= c[r+1][j]*In[(vol-r+j)%size];
		left[r]+=c[r][j]*In[(vol-r+j)%size];
	      }
	   
	  } // 36 flops
	// regularity coefficients:
	double beta[3];
	beta[0]= b0* std::pow(In[vol]-2.0*In[(vol+1)%size]+In[(vol+2)%size],2)+
	  b1*std::pow(3.*In[vol]-4.*In[(vol+1)%size]+In[(vol+2)%size],2);
	beta[1]=b0* std::pow(In[index(vol-1)]-2.0*In[vol]+In[(vol+1)%size],2)+
	  b1*std::pow(In[index(vol-1)]-In[(vol+1)%size],2);
	beta[2]=b0* std::pow(In[index(vol-2)]-2.0*In[index(vol-1)]+In[vol],2)+
	  b1*std::pow(In[index(vol-2)]-4.*In[index(vol-1)]+3*In[vol],2);
	// 15 flops.
	double alpharight[3],alphaleft[3],sright=0,sleft=0;
	for(int r=0;r<3;r++)
	  {
	    alpharight[r]=dright[r]/std::pow(epsilon+beta[r],2);
	    sright+=alpharight[r];
	  } //3*4 = 12 flops.
	for(int r=0;r<3;r++)
	  {
	    alphaleft[r]=dleft[r]/std::pow(epsilon+beta[r],2);
	    sleft+=alphaleft[r];
	  } // 12 flops
	double recleft=0,recright=0;
	for(int r=0;r<3;r++) recleft+= alphaleft[r]*left[r]; // 12 flops
	for(int r=0;r<3;r++) recright+= alpharight[r]*right[r];// 12 flops

	// reconstructed values:
	reconstructed[2*vol]  = recleft/sleft; // 1 flop
	reconstructed[2*vol+1]= recright/sright; //1 flop
      } //all together: size*(36+ 4*12 +2) flops = 86* size flops.
    // compute the numerical fluxes at boundaries:
    for(int vol=0;vol<size;vol++)
      numflux[vol]=
	F( reconstructed[2*vol+1], reconstructed[2*((vol+1)%size)]);
    // size* nflops_F
    // now, return RHS to solver:
    Out[0]= h1*(numflux[0]-numflux[size-1]);
    for(int vol=1;vol<size;vol++)
      Out[vol]=h1*(numflux[vol]-numflux[(vol-1)%size]);
    // 2*(size+1) flops
  }
};
