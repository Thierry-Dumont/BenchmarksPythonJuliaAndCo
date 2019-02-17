#include <iostream>
#include <algorithm>
#include <cmath>
#include <map>
#include <string>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>
#include <fstream>
using namespace std;

double get_time() {
    struct timeval tv;
    gettimeofday(&tv,0);
    return (double) tv.tv_sec+tv.tv_usec*1e-6;
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
  auto  F  = [] (double  x) {return  exp(-x)*x*x; };
  double t1=get_time();
  auto sum= trapz(F,0.,1.,100000000);
  double t2=get_time()-t1;
  cout<<"computing time: "<<t2<<endl;
  cout<<sum<<endl;

  auto G= [](double  x) {return x<0.5? -exp(-x)*x*x: exp(x)*x*x;};
  t1=get_time();
  sum= trapz(G,0.,1.,100000000);
  t2=get_time()-t1;
  cout<<"computing time: "<<t2<<endl;
  cout<<sum<<endl;
}
