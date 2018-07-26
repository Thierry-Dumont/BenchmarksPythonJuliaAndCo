#include <iostream>
#include <algorithm>
#include <string>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>
#include <fstream>
#include <cmath>
#include <ctime>
#include <array>
#include "xtensor/xtensor.hpp"
#include "xtensor/xview.hpp"
using namespace std;
typedef xt::xtensor<double,2> Array;

double get_time() {
    struct timeval tv;
    gettimeofday(&tv,0);
    return (double) tv.tv_sec+tv.tv_usec*1e-6;
}
clock_t ck() {return clock();}
string host()
{
  char hostnameC[HOST_NAME_MAX];
  gethostname(hostnameC, HOST_NAME_MAX);
  return  string(hostnameC);
}

void Init(Array&  X,double L,int size)
{

  double h=L/size;
  for(int i=0;i<size;i++)
    for(int j=0;j<size;j++)
      if(i>size/8 && i<size/2+size/8)
	X(i,j)=1.-2*(i-size/8)*h/L;
      else
	X(i,j)=0.0;
}
void lapl_2(int size,Array& In,Array& Out)
{

  double h2= 1./(size*size);

  xt::view(Out, xt::range(1,size-1), xt::range(1,size-1))=
    h2*(
	view(In, xt::range(0,size-2), xt::range(1,size-1))+
	view(In, xt::range(1,size-1), xt::range(0,size-2))+
	-4.0*view(In, xt::range(1,size-1), xt::range(1,size-1))+
	view(In, xt::range(2,size), xt::range(1,size-1))+
	view(In, xt::range(1,size-1), xt::range(2,size))
	);
}
double  dotest(std::size_t size)
{
  std::array<size_t, 2> shape = { size,size  };
  Array A(shape), B(shape);
  Init(A,1.,size); Init(B,1.,size);
  double T=0;
  double Tnew=std::pow(10.,20);
  int iter=1000;
  bool ok=false;
  do
    {
      double t1=get_time();
      for(int i=0;i<iter;i++)
	lapl_2(size,A,B);
      Tnew=(get_time()-t1);
      ok= std::abs(Tnew-2*T)/Tnew<0.1||iter>1000000;

      T=Tnew;
      if(!ok) iter*=2;
    }
  while(!ok);
  return T/iter;
}
  
int main()
{
 

  auto hostname = host();
  cout<<"hostname: "<<hostname<<endl;
  ofstream fb; fb.open("../RunningOn"+hostname+"_lapl_2");
  std::size_t sizemax=2049;
  std::size_t size=32;
  while(size<sizemax)
    {
      auto T=dotest(size);
      double flops=5*std::pow(size-2,2)/T;
      cout<<size<<" "<<T<<", Gflops/s: "<<flops*pow(10,-9)<<endl;
      fb<<size<<" "<<T<<endl;
      size*=2;
    }
  fb.close();
  cout<<"end"<<endl;
}
