#include "PreSparse.hpp"
#include "PreLapl.hpp"
#include "Csr.hpp"
#include <math.h>  
#include <iostream>
#include <tuple>
#include <algorithm> 
//#include <memory>
#include <iomanip>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;
// Clock!
class Mtime
{
  high_resolution_clock::time_point t1 ;
  public:
  // Initialize (start time!)
  void start()
  {
    t1= high_resolution_clock::now();
  }
  // Get duration since timer was started, in seconds.
  double sec() const
  {
    high_resolution_clock::time_point t2= high_resolution_clock::now();
    return 1.e-9*
      static_cast<double>(duration_cast<nanoseconds>(t2 - t1 ).count());
  }
};
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
template<int dim> tuple<double,double,int,int> dotest_arrays(int size)
{
  int order,nc;
  // build matrix:
  // std::shared_ptr<int[]> row;
  // std::shared_ptr<int[]> col;
  // std::shared_ptr<double[]> v;
  int *row;
  int *col;
  double *v;

  Mtime T;
  
  T.start();
  std::tie(order,nc)= PreLapl<dim>(row,col,v,size);
  Csr M(row,col,v,order,nc);
  auto c1=T.sec();
  
  
  //auto In=std::make_unique<double[]>(order);
  //auto Out=std::make_unique<double[]>(order);
  auto In= new double[order];
  auto Out=new double[order];
  
  Init<dim>(In,size); Init<dim>(Out,size);
  // product:
  int iterm=10;

  double c2;
 
  do
    {
      T.start();
      for(int it=0;it<iterm;it++)
	M.prod(In,Out);
      c2=T.sec();
      //cout<<iterm<<" "<<c2<<endl;
      iterm*=1+0.5/c2;
      
    }
  while(c2<0.5);
  //cout<<iterm<<endl;
  delete[] In; delete[] Out;
  return make_tuple(c1,c2/(iterm-1),order,nc);
}
template<int dim> tuple<double,double,int,int> dotest_map(int size)
{
  
  int ni,nj,sizem;
  
  Mtime T;
  // build the matrix:

  T.start();
  PreSparse P;
  PreLapl<dim>(P,size);
 
  tie(ni,nj,sizem)=P.sizes();
 
  Csr M(P);

  auto c2=T.sec();

  
  P.purge();
  
  //auto In=std::make_unique<double[]>(ni);
  //auto Out=std::make_unique<double[]>(ni);
  auto In=new double[ni];
  auto Out=new double[ni];
  
  Init<dim>(In,size); Init<dim>(Out,size);
  // product:
  int iterm=1;

  double c4;
  do
    {
      T.start();
      for(int it=0;it<iterm;it++)
	M.prod(In,Out);
      c4=T.sec();
      iterm*=2;
  
    }
  while(c4<0.1);//at least run 0.1 second

  delete[] In; delete[] Out;
  return make_tuple(c2,c4/(iterm-1),ni,sizem);
}
void banner()
{
  cout<<left<<setw(9)<<"size"<<left<<setw(15)<<"order"<<setw(15)<<"nc"<<" ";
  cout<<left<<setw(15)<<"T.b."<<setw(15)<<"T.p."<<setw(15)
      <<"T.b/nc"<<setw(13)<<"T.m./nc"<<setw(10)<<"Gflops/s"<<endl;
  for(int i=0;i<107;i++)
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
  double tb,tm;
  int order,ncoefs;
  fb.open("../RunningOn"+hostname+"-2");
  banner();
  for(int size=sizemin;size<=sizemax;size*=2)
    {
      tie(tb,tm,order,ncoefs) = dotest_arrays<2>(size);
      

      long int flops=2*ncoefs-order;
      double Gflopss=   pow(10,-9)*flops/tm;
      cout<<left<<setw(9)<<size<<left<<setw(15)<<order<<setw(15)<<ncoefs<<" ";
      cout<<left<<setw(15)<<tb<<setw(15)
	  <<tm<<setw(15)
	  <<static_cast<double>(tb)/ncoefs<<setw(13)
	  <<static_cast<double>(tm)/ncoefs<<setw(10)<<Gflopss<<
	endl;
      fb<<size<<" "<<order<<" "<<ncoefs<<" "
	<<tb<<" "<<tm<<endl;
    }
  fb.close();
  
  cout<<endl<<"3d:"<<endl;
  sizemin=16; sizemax=256;
  fb.open("../RunningOn"+hostname+"-3");
  banner();
 
  for(int size=sizemin;size<=sizemax;size*=2)
    {
      tie(tb,tm,order,ncoefs) = dotest_arrays<3>(size);
      
      long int flops=2*ncoefs-order;
      double Gflopss=   pow(10,-9)*flops/tm;
      cout<<left<<setw(9)<<size<<left<<setw(15)<<order<<setw(15)<<ncoefs<<" ";
      cout<<left<<setw(15)<<tb<<setw(15)
	  <<tm<<setw(15)
	  <<static_cast<double>(tb)/ncoefs<<setw(13)
	  <<static_cast<double>(tm)/ncoefs<<setw(10)<<Gflopss<<
	endl;
      fb<<size<<" "<<order<<" "<<ncoefs<<" "
	<<tb<<" "<<tm<<endl;
    }
  fb.close();
  cout<<"end."<<endl;
}
