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
struct index{
  const int size;
  index(int _size):size(_size){}
  inline int operator()(int i,int j){return i*size+j;}
};
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
  index ii(size);
  double h=L/size;
  for(int i=0;i<size;i++)
    for(int j=0;j<size;j++)
      if(i>size/8 && i<size/2+size/8)
	X[ii(i,j)]=1.-2*(i-size/8)*h/L;
      else
	X[ii(i,j)]=0.0;
}
void lapl_2(int size,std::unique_ptr<double[]>& In,
	    std::unique_ptr<double[]>& Out)
{
  index ii(size);
  double h2= 1./(size*size);
  for(int i=1;i<size-1;i++)
    for(int j=1;j<size-1;j++)
      Out[ii(i,j)]=
	h2*(In[ii(i-1,j)] + In[ii(i,j-1)]-
	    4.0*In[ii(i,j)]+
	    In[ii(i+1,j)]+In[ii(i,j+1)]);
  
}
double  dotest(int size)
{
  auto A=std::make_unique<double[]>(size*size);
  auto B=std::make_unique<double[]>(size*size);
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
      Tnew=(get_time()-t1)/iter;
      A.swap(B);
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
  ofstream fb; fb.open("../RunningOn"+hostname+"_lapl_2");
  int sizemax=2049;
  int size=8;
  while(size<sizemax)
    {
      auto T=dotest(size);
      double flops=6*std::pow(size-2,2)/T;
      cout<<size<<" "<<T<<", Gflops/s: "<<flops*pow(10,-9)<<endl;
      fb<<size<<" "<<T<<endl;
      size*=2;
    }
  fb.close();
  cout<<"end"<<endl;
}
