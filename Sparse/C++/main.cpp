#include "PreSparse.hpp"
#include "PreLapl.hpp"
#include "Csr.hpp"
#include <math.h>  
#include <iostream>
#include <tuple>
//#include <memory>
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
//template<int dim> void Init(std::unique_ptr<double[]>& X,int size)
template<int dim> void Init(double X[],int size)
{
  int sized=pow(size,dim);
  double v=1.;
  for(int i=0;i<sized;i++)
    {
      X[i]=v;
      v=-v;
    }
}
template<int dim> tuple<clock_t,clock_t,int,int> dotest_arrays(int size)
{
  int order,nc;
  // build matrix:
  // std::shared_ptr<int[]> row;
  // std::shared_ptr<int[]> col;
  // std::shared_ptr<double[]> v;
  int *row;
  int *col;
  double *v;
  
  auto c1=ck();
  std::tie(order,nc)= PreLapl<dim>(row,col,v,size);
  Csr M(row,col,v,order,nc);
  auto c2=ck()-c1;
  
  
  //auto In=std::make_unique<double[]>(order);
  //auto Out=std::make_unique<double[]>(order);
  auto In= new double[order];
  auto Out=new double[order];
  
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
      iterm*=2;
    }
  while(c4<CLOCKS_PER_SEC/100);

  delete[] In; delete[] Out;
  return make_tuple(order,nc,c2,c4/(iterm-1));
}
template<int dim> tuple<clock_t,clock_t,int,int> dotest_map(int size)
{
  
  int ni,nj,sizem;

  // build the matrix:
  auto c1=ck();
  PreSparse P;
  PreLapl<dim>(P,size);
 
  tie(ni,nj,sizem)=P.sizes();
 
  Csr M(P);

  auto c2=ck()-c1;

  
  P.purge();
  
  //auto In=std::make_unique<double[]>(ni);
  //auto Out=std::make_unique<double[]>(ni);
  auto In=new double[ni];
  auto Out=new double[ni];
  
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
      iterm*=2;
    }
  while(c4<CLOCKS_PER_SEC/100);

  delete[] In; delete[] Out;
  return make_tuple(ni,sizem,c2,c4/(iterm-1));
}
void banner()
{
  cout<<left<<setw(9)<<"size"<<left<<setw(9)<<"order"<<setw(10)<<"nc"<<" ";
  cout<<left<<setw(10)<<"T.b."<<setw(10)<<"T.p."<<setw(15)
      <<"T.b/nc"<<setw(13)<<"T.m./nc"<<setw(20)<<"Gflops/s"<<endl;
  for(int i=0;i<85;i++)
    cout<<"-";
  cout<<endl;
}  
int main()
{

  // If you want to test building matrices with maps, replace dotest_arrays
  // by dotest_map below (twice).
  
  auto hostname = host();
  cout<<"hostname: "<<hostname<<endl<<endl;

  cout<<left<<setw(6)<<"size"<<": "<<"size of the grid."<<endl;
  cout<<left<<setw(6)<<"order"<<": "<<"order of the matrix."<<endl;
  cout<<left<<setw(6)<< "nc"<<": "<<"number of ceofficients."<<endl;
  cout<<left<<setw(6)<< "T.b"<<": "<<"time to build the matrix (s)."<<endl;
  cout<<left<<setw(6)<< "T.p"<<": "<<"time for matrix x vector product (s)."
      <<endl<<endl;
  
  ofstream fb; 
  
  int sizemin=32,sizemax=2048;
  
  cout<<"2d:"<<endl;
  clock_t tb,tm;
  int order,ncoefs;
  fb.open("../RunningOn"+hostname+"-2");
  banner();
  for(int size=sizemin;size<=sizemax;size*=2)
    {
      tie(order,ncoefs,tb,tm) = dotest_arrays<2>(size);
      
      auto tbd=static_cast<double>(tb);
      auto tmb=static_cast<double>(tm)/CLOCKS_PER_SEC;
      long int flops=2*ncoefs-order;
      double Gflopss= pow(10,-9)*flops/tmb;
      cout<<left<<setw(9)<<size<<left<<setw(9)<<order<<setw(10)<<ncoefs<<" ";
      cout<<left<<setw(10)<<tbd/CLOCKS_PER_SEC<<setw(10)
	  <<tmb<<setw(15)
	  <<static_cast<double>(tb)/ncoefs<<setw(13)
	  <<static_cast<double>(tm)/ncoefs<<setw(10)<<Gflopss<<
	endl;
      fb<<size<<" "<<order<<" "<<ncoefs<<" "
	<<tbd/CLOCKS_PER_SEC<<" "<<tmb<<endl;
    }
  fb.close();
  
  cout<<endl<<"3d:"<<endl;
  sizemin=16; sizemax=256;
  fb.open("../RunningOn"+hostname+"-3");
  banner();
  for(int size=sizemin;size<=sizemax;size*=2)
    {
      tie(order,ncoefs,tb,tm) =dotest_arrays<3>(size);
      //tie(order,ncoefs,tb,tm) =dotest_map<3>(size);
      auto tbd=static_cast<double>(tb);
      auto tmb=static_cast<double>(tm)/CLOCKS_PER_SEC;
      long int flops=2*ncoefs-order;
      double Gflopss= pow(10,-9)*flops/tmb;
      cout<<left<<setw(9)<<size<<left<<setw(9)<<order<<setw(10)<<ncoefs<<" ";
      cout<<left<<setw(10)<<tbd/CLOCKS_PER_SEC<<setw(10)
	  <<tmb<<setw(15)
	  <<static_cast<double>(tb)/ncoefs<<setw(13)
	  <<static_cast<double>(tm)/ncoefs<<setw(10)<<Gflopss<<
	endl;
      fb<<size<<" "<<order<<" "<<ncoefs<<" "
	<<tbd/CLOCKS_PER_SEC<<" "<<tmb<<endl;
      
    }
  fb.close();
  cout<<"end."<<endl;
}
