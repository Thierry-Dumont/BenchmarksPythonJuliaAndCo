#include <iostream>
#include <algorithm>
#include <cmath>
#include <map>
#include <string>
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
  return  string(hostnameC);
}

template<typename Fonc> double trapz(Fonc &F,double a, double b, int n)
{
  auto h=(b-a)/n;
  auto sum=0.5*(F(a)+F(b));
  for(int i=1;i<=n;i++)
    sum+=F(i*h);
  
  return sum*h;
}
int main()
{
  auto hostname = host();
  cout<<"hostname: "<<hostname<<endl;
  
  Mtime T;
  
  const int loops=10000;
  double sum;
  
  ofstream f; f.open("../RunningOn"+hostname);
  
  auto  F  = [] (double  x) {return  exp(-x)*x*x; };
  T.start();
  for(int i=0;i<loops;i++)
    sum= trapz(F,0.,1.,1000);
  double t2=T.sec()/loops;
  cout<<"computing time: "<<t2<<endl;
  cout<<sum<<endl;
  f<<"f: "<<t2<<endl;
  
  auto G= [](double  x) {return x<0.5? -exp(-x)*x*x: exp(x)*x*x;};
  T.start();
  for(int i=0;i<loops;i++)
    sum= trapz(G,0.,1.,1000);
  t2=T.sec()/loops;
  cout<<"computing time: "<<t2<<endl;
  cout<<sum<<endl;
  f<<"g: "<<t2<<endl;

  f.close();
}
