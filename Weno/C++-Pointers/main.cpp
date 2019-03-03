#include "GodunovFlux.hpp"
#include "LaxFriedrichsFlux.hpp"
#include "Burghers.hpp"
#include "Convection.hpp"
#include "Weno.hpp"
#include "RK3TVD.hpp"
#include <iostream>
#include <algorithm>
#include <map>
#include <string>
#include <unistd.h>
#include <limits.h>
#include <fstream>
#include <cmath>
#include <typeinfo>
#include <utility>
#include <chrono>
//#define DO_GNUPLOT_FILES
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
template<typename T> pair<string,string> how_computed()
{
  string s=typeid(T).name();
  string problem=s.find("Burghers")!=std::string::npos? "Burghers":"Convection";
  string flux=s.find("Godunov")!=std::string::npos?"Godunov": "Lax-Friedrichs";
  return make_pair(problem,flux);
}
void Init(double *  X,double L,int size)
{
  double h=L/size;
  for(int i=0;i<size;i++)
    if(i>size/8 && i<size/2+size/8)
      X[i]=1.-2*(i-size/8)*h/L;
    else
      X[i]=0.0;
}
int main()
{
  // define problem:
  typedef Burghers Problem;
  //typedef Convection Problem;

  // define numerical flux:
  typedef GodunovFlux<Problem> NumFlux;
  //typedef LaxFriedrichsFlux<Problem> NumFlux;
  
  const int size=1000;
  const double L=1.0;
  double dt=0.8/size;
  double T=1.;


  double fparam[1];
  fparam[0]=1.;
  auto hostname = host();
  cout<<"hostname: "<<hostname<<endl;

  auto pc=how_computed<NumFlux>();
  cout<<pc.first<<" solved with "<<pc.second<<" flux." <<endl;
  
  int nsteps=T/dt;
  cout<<"size= "<<size<<" dt= "<<dt<<" nsteps= "<<nsteps<<endl;
  double * InOut=new double[size];
  
#ifdef DO_GNUPLOT_FILES
 
  int ngp= max(1,nsteps/100);
  cout<<endl<<"We are going to create files to visualize the solution: ";
  cout<<"DISABLE THiS FOR BENCHMARKING!"<<endl<<endl;
  cout<<"One plot every "<<ngp<<" steps. We will do about "<<nsteps<<
    " steps."<<endl;
  ofstream f,gpfile;
  gpfile.open("gpfile");
  
#endif

  RK3TVD<Weno<NumFlux> > RFL(size,L,fparam);
  
  Init(InOut,L,size);// initial condition.

  double t=0.;

  Mtime Tm;
  
  int step=0;
  cout<<"start..."<<endl;
  
  Tm.start();
  while(t<T)
    {
      RFL.step(InOut,dt);
      
#ifdef DO_GNUPLOT_FILES
      
      if(step%ngp ==0)
	{
	  gpfile<<"plot "<<"\"results"+to_string(step)+"\" with lines"<<endl;
	  gpfile<<"pause 0.1"<<endl;
	  f.open("results"+to_string(step));
	  for(int i=0;i<size;i++)
	    f<<InOut[i]<<endl;
	  f.close();
	}
      
 #endif
      ++step;
      t+=dt;
 
    }
  double t2=Tm.sec();

  
  cout<<"Computing time: "<<t2<<" seconds."<<endl;
  cout<<"nsteps: "<<step<<endl;
#ifdef DO_GNUPLOT_FILES 
  gpfile.close();
#else
  // make one file, with the final computation
  ofstream f;
  f.open("gp");
  for(int i=0;i<size;i++)
    f<<InOut[i]<<endl;
  f.close();
  cout<<"A file 'gp' with the final solution was created."<<endl; 
#endif


  ofstream fb; fb.open("../RunningOn"+hostname);
  fb<<pc.first<<" "<<pc.second<<endl;
  fb<<t2<<endl;
  fb.close();

  delete[] InOut;
  cout<<"end"<<endl;
}
