#include <iostream>
#include <algorithm>
#include <string>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>
#include <fstream>
#include <cmath>
#include <ctime>
#include <memory>
using namespace std;

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

void Init(std::unique_ptr<double[]>&  X,double L,int size)
{
  double h=L/size;
  for(int i=0;i<size;i++)
    if(i>size/8 && i<size/2+size/8)
      X[i]=1.-2*(i-size/8)*h/L;
    else
      X[i]=0.0;
}
void cl(int size,std::unique_ptr<double[]>& A,std::unique_ptr<double[]>& B,
	std::unique_ptr<double[]>& C,std::unique_ptr<double[]>& D)
{
  for(int i=0;i<size;i++)
    A[i]=1.7*B[i]-0.8*C[i]-0.9*D[i];
  
  
}
double  dotest(int size)
{
  auto A=std::make_unique<double[]>(size);
  auto B=std::make_unique<double[]>(size);
  auto C=std::make_unique<double[]>(size);
  auto D=std::make_unique<double[]>(size);
  Init(A,1.,size); Init(B,1.,size); Init(C,1.,size); Init(D,1.,size); 
  double T=0;
  double Tnew=std::pow(10.,20);
  int iter=10000;
  bool ok=false;
  do
    {
      double t1=get_time();
      for(int i=0;i<iter;i++)
	cl(size,A,B,C,D);
      Tnew=(get_time()-t1)/iter;
      A.swap(D);
      ok= std::abs(Tnew-T)<0.01;
      T=Tnew;
      iter*=10;
    }
  while(!ok);
  return T;
}
  
int main()
{
  auto hostname = host();
  cout<<"hostname: "<<hostname<<endl;
  ofstream fb; fb.open("../RunningOn"+hostname+"_cl");
  
  int sizemax=std::pow(10,6);
  int size=4;
  while(size<sizemax)
    {
      auto T=dotest(size);
      double flops=size*5/T;
      cout<<size<<" "<<T<<", Gflops/s: "<<flops*std::pow(10,-9)<<endl;
      fb<<size<<" "<<T<<endl;
      size*=2;
    }
  fb.close();
  cout<<"end"<<endl;
}
