#include "rando.hpp"
#include "Stiffness.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <unistd.h>
#include <limits.h>
#include <cmath>
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
  return  string(hostnameC);
}
void RandomTriangle(rando& R, double x[], double y[])
{
  // domain is [0,10]x[0,10]
  for(int i=0;i<3;i++)
    x[i]= R.fv(10.);
  for(int i=0;i<3;i++)
    y[i]= R.fv(10.);
}
int main()
{

  auto hostname = host();
  cout<<"hostname: "<<hostname<<endl;
  
  const long int ntri=std::pow(10,6);
  
  double x[3],y[3]; // triangle summits.
  double mat[21];
  rando R;

  Stiffness S;

  //
  cout<<"\nVerify that, on the reference element, we are coherent with sage"
      <<endl<<"(see ../sage/):"<<endl;

  x[0]=0.0; x[1]=1.0; x[2]=0.0;
  y[0]=0.0; y[1]=0.0; y[2]=1.0;
  S(x,y,mat);
  for(int i=0;i<6;i++)
    {
      for(int j=0;j<=i;j++)
	cout<<std::setw(10) <<mat[ i*(i+1)/2+j ]<<" ";
      cout<<endl;
    }
  cout<<"\nAlso, nothing should change when dilating the element:"<<endl;
  for(int i=0;i<3;i++)
    {
      x[i]*=2;
      y[i]*=2;
    }
  S(x,y,mat);
  for(int i=0;i<6;i++)
    {
      for(int j=0;j<=i;j++)
	cout<<std::setw(10) <<mat[ i*(i+1)/2+j ]<<" ";
      cout<<endl;
    }
  cout<<"\nNow, start the benchmark:"<<endl;

  Mtime Tm;
  
  cout<<ntri<<" triangles."<<endl;
  Tm.start();
  for(long int tri=0;tri<ntri;tri++)
    {
      RandomTriangle(R,x,y);
      S(x,y,mat);
    }
  auto T= Tm.sec();
  cout<<"first phase: "<<T<<" seconds."<<endl;
					 
  Tm.start();	
  for(long int tri=0;tri<ntri;tri++)
    {
      RandomTriangle(R,x,y);
    }
    auto t2= Tm.sec();
  cout<<"second phase: "<<t2<<" seconds" <<endl;
 
  T-=t2;
  cout<<"Total time: "<<T <<" seconds."<<
    endl;
  double ttri= T /ntri;
  cout<<"Time by triangle: "<<ttri<<" second."<<endl;

  cout<<S.flops/ttri<<" Gflops/s."<<endl;

  ofstream f; f.open("../RunningOn"+hostname);
  f<<ttri<<endl;
  f.close();
  cout<<endl;
  cout<<"end."<<endl;
}
