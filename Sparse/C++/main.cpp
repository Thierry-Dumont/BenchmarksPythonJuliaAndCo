#include "PreSparse.hpp"
#include "PreLapl.hpp"
#include "Csr.hpp"
#include <iostream>
#include <tuple>
#include <memory>
using namespace std;
clock_t ck() {return clock();}
template<int dim> tuple<clock_t,clock_t,int,int> dotest(int size)
{
  int ni,nj,sizem;

  // construction of the matrix:
  auto c1=ck();
  PreSparse P;
  PreLapl<dim>(P,size);
  tie(ni,nj,sizem)=P.sizes();
  //cout<<ni<<" "<<nj<<" "<<sizem<<endl;
  Csr M(P);
  auto c2=ck()-c1;
  //
  
  P.purge();
  
  auto In=std::make_unique<double[]>(ni);
  auto Out=std::make_unique<double[]>(ni);

  // product:
  auto c3=ck();
  M.prod(In,Out);
  auto c4=ck()-c3;

  return make_tuple(ni,sizem,c2,c4);
}
int main()
{
  //CLOCKS_PER_SEC
  int sizemin=32,sizemax=2048;
  cout<<"2d:"<<endl;
  clock_t tb,tm;
  int order,ncoefs;
  for(int size=sizemin;size<=sizemax;size*=2)
    {
      tie(order,ncoefs,tb,tm) = dotest<2>(size);
      cout<<size<<" "<<order<<" "<<ncoefs<<" ";
      cout<<static_cast<double>(tb)/CLOCKS_PER_SEC<<" "
	  <<static_cast<double>(tm)/CLOCKS_PER_SEC<<" "
	  <<static_cast<double>(tb)/ncoefs<<" "
	  <<static_cast<double>(tm)/ncoefs<<" "<<
	endl;
    }
  cout<<endl<<"3d:"<<endl;
  sizemin=16; sizemax=128;
  for(int size=sizemin;size<=sizemax;size*=2)
    {
      tie(order,ncoefs,tb,tm) =dotest<3>(size);
      cout<<size<<" "<<order<<" "<<ncoefs<<" ";
      cout<<static_cast<double>(tb)/CLOCKS_PER_SEC<<" "
	  <<static_cast<double>(tm)/CLOCKS_PER_SEC<<" "
	  <<static_cast<double>(tb)/ncoefs<<" "
	  <<static_cast<double>(tm)/ncoefs<<" "<<
	endl;
    }
    cout<<"end."<<endl;
}
