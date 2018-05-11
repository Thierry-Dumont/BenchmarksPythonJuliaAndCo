#pragma once
#include <iostream>
using namespace std;
////////////////////////////////////////////////////////////////////////
/// Stiffness matrix for P2  finite element in dimension 2 (Laplacian).
////////////////////////////////////////////////////////////////////////
class Stiffness
{
  double ji[4];
  double grads[36];
  // gradients of basis functions on the reference element, at the
  // middle of the edges.
  double gq[36]={-1, -1, 1, 1, -1, -1, 1, 0, 1, 0, -1, 0, 0, -1,
		 0, 1, 0, 1, 0, -2, -2, -2, 2, 0, 0, 2, 2, 2, 2,
		 0, 0, 2, -2, -2, -2, 0};
  // we will compute only an half part (with diagonal) of the matrix (symetry!)
  inline int ind(int i,int j){return i*(i+1)/2+j;}
  // transformation current element -> reference lement (x by determinant)/
  inline void JinvDetTrans(double x[],double y[])
  {
    ji[0]=-y[0] + y[2] ; ji[1]= y[0] - y[1];
    ji[2] = x[0] - x[2]; ji[3]= -x[0] + x[1];
    // 4 flops
  }
public:
  Stiffness()//empty constructor
  {
  }
  ~Stiffness()//empty destructor
  {
  }
  // compute the stiffness matrix.
  // x[3], y[3]: IN, the trangle.
  // m[21] : OUT, the computed matrix.
  void operator()(double x[],double y[],double m[])
  {
    
    JinvDetTrans(x,y);
    for(int f=0;f<6;f++)
      for(int p=0;p<3;p++)
	{
	  int d=6*f+2*p;
	  grads[d]  = ji[0]*gq[d]+ji[1]*gq[d+1];
	  grads[d+1]= ji[2]*gq[d]+ji[3]*gq[d+1];
	} //18 * 6 = 108 flops.
    double det= -(x[1] - x[2])*y[0] + (x[0] - x[2])*y[1] - (x[0] - x[1])*y[2];
    // det: 8 flops.
    double dv=1.0/(6.0*det); // 1 flop
    for(int i=0;i<6;i++)
      for(int j=0;j<=i;j++)
	{
	  double s=0;
	  for(int k=0;k<3;k++)
	    s+=grads[6*i+2*k]*grads[6*j+2*k]+grads[6*i+2*k+1]*grads[6*j+2*k+1];
	  m[ind(i,j)]=s;
	}// 21* 4 = 84 flops.
    
    for(int i=0;i<21;i++) m[i]*=dv; //21 flops

    //total:  4+18*6+ 8 + 84 +1+21 = 226 flops.
 
  }
  static const int  flops = 226; 
};
