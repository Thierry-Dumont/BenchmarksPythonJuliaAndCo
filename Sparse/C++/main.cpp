#include "PreSparse.hpp"
#include "PreLapl.hpp"
#include "Csr.hpp"
#include <math.h>  
#include <iostream>
#include <tuple>
#include <memory>
#include <iomanip>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>
#include <fstream>

using namespace std;
clock_t ck() {return clock();}
//get... time.
double get_time() {
  struct timeval tv;
  gettimeofday(&tv,0);
  return (double) tv.tv_sec+tv.tv_usec*1e-6;
}
string host()
{
  char hostnameC[HOST_NAME_MAX];
  gethostname(hostnameC, HOST_NAME_MAX);
  return string(hostnameC);
}
template<int dim> void Init(std::unique_ptr<double[]>& X,int size)
{
  int sized=pow(size,dim);
  double v=1.;
  for(int i=0;i<sized;i++)
    {
      X[i]=v;
      v=-v;
    }
}
template<int dim> tuple<clock_t,clock_t,int,int> dotest(int size)
{
  int ni,nj,sizem;

  // construction of the matrix:
  auto c1=ck();
  PreSparse P;
  PreLapl<dim>(P,size);
  // if(dim==2 && size==32)
  //   {P.print();exit(0);}

  tie(ni,nj,sizem)=P.sizes();
 
  Csr M(P);
  //M.print();exit(0);
  auto c2=ck()-c1;
  //
  
  P.purge();
  
  auto In=std::make_unique<double[]>(ni);
  auto Out=std::make_unique<double[]>(ni);
  Init<dim>(In,size); Init<dim>(Out,size);
  // product:
  int iterm=1;
  clock_t c4;
  do
    {
      auto c3=ck();
      for(int it=0;it<iterm;it++)
	M.prod(In,Out);
      c4=ck()-c3;
      iterm++;
    }
  while(c4<CLOCKS_PER_SEC/100);
  //cout<<iterm-1<<endl;
  return make_tuple(ni,sizem,c2,c4/(iterm-1));
}
void banner()
{
  cout<<left<<setw(9)<<"size"<<left<<setw(9)<<"order"<<setw(10)<<"nc"<<" ";
  cout<<left<<setw(10)<<"T.m."<<setw(10)<<"T.p."<<setw(10)
      <<"T.m/nc"<<setw(13)<<"T.p./nc"<<setw(20)<<"Gflops/s"<<endl;
  for(int i=0;i<8;i++)
    cout<<"----------";
  cout<<endl;
}  
int main()
{
  auto hostname = host();
  cout<<"hostname: "<<hostname<<endl;

  ofstream fb; fb.open("../RunningOn"+hostname);
  
  int sizemin=32,sizemax=2048;
  cout<<"2d:"<<endl;
  clock_t tb,tm;
  int order,ncoefs;
  banner();
  for(int size=sizemin;size<=sizemax;size*=2)
    {
      tie(order,ncoefs,tb,tm) = dotest<2>(size);

      cout<<left<<setw(9)<<size<<left<<setw(9)<<order<<setw(10)<<ncoefs<<" ";
      auto tbd=static_cast<double>(tb);
      auto tmb=static_cast<double>(tm)/CLOCKS_PER_SEC;
      long int flops=2*ncoefs-order;
      double Gflopss= pow(10,-9)*flops/tmb;
      cout<<left<<setw(10)<<tbd/CLOCKS_PER_SEC<<setw(10)
	  <<tmb<<setw(10)
	  <<static_cast<double>(tb)/ncoefs<<setw(13)
	  <<static_cast<double>(tm)/ncoefs<<setw(10)<<Gflopss<<
	endl;
      fb<<"2 "<<" "<<size<<" "<<order<<" "<<ncoefs<<" "
	<<tbd/CLOCKS_PER_SEC<<" "<<tmb/CLOCKS_PER_SEC<<" "
	<<static_cast<double>(tb)/ncoefs<<" "<<static_cast<double>(tm)/ncoefs
	<<" "<<Gflopss<<endl;
    }
  cout<<endl<<"3d:"<<endl;
  sizemin=16; sizemax=128;
  banner();
  for(int size=sizemin;size<=sizemax;size*=2)
    {
      tie(order,ncoefs,tb,tm) =dotest<3>(size);
      auto tbd=static_cast<double>(tb);
      auto tmb=static_cast<double>(tm)/CLOCKS_PER_SEC;
      long int flops=2*ncoefs-order;
      double Gflopss= pow(10,-9)*flops/tmb;
      cout<<left<<setw(9)<<size<<left<<setw(9)<<order<<setw(10)<<ncoefs<<" ";
      cout<<left<<setw(10)<<tbd/CLOCKS_PER_SEC<<setw(10)
	  <<tmb/CLOCKS_PER_SEC<<setw(10)
	  <<static_cast<double>(tb)/ncoefs<<setw(13)
	  <<static_cast<double>(tm)/ncoefs<<setw(10)<<Gflopss<<
	endl;
      fb<<"3 "<<" "<<size<<" "<<order<<" "<<ncoefs<<" "
	<<tbd/CLOCKS_PER_SEC<<" "<<tmb/CLOCKS_PER_SEC<<" "
	<<static_cast<double>(tb)/ncoefs<<" "<<static_cast<double>(tm)/ncoefs
	<<" "<<Gflopss<<endl;
      
    }
  fb.close();
  cout<<"end."<<endl;
}
