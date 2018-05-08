#include "rando.hpp"
#include "Stiffness.hpp"
#include <iostream>
#include <algorithm>
#include <map>
#include <string>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>
#include <fstream>
#include <cmath>
#include <ctime>
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
  
  map<int,double> times;

  const long int ntri=std::pow(10,7);
  
  double x[3],y[3]; // triangle summits.
  double mat[36];
  rando R;

  Stiffness S;
  //auto t1= get_time();
  auto t1=ck();
  for(long int tri=0;tri<ntri;tri++)
    {
      RandomTriangle(R,x,y);
      S(x,y,mat);
    }
  auto T= ck()- -t1;
  cout<<"first phase: "<<(double) T/ CLOCKS_PER_SEC<<endl;
 
  t1= ck();//get_time();

  for(long int tri=0;tri<ntri;tri++)
    {
      RandomTriangle(R,x,y);
    }
  auto t2= ck()-t1;//get_time()-t1;
  cout<<"second phase: "<<(double) t2/ CLOCKS_PER_SEC <<endl;
 
  //cout<<tri<<" "<<ntri<<endl;
  T-=t2;
  cout<<"T: "<<(double) T/ CLOCKS_PER_SEC <<endl;
  cout<<endl;
  cout<<"end"<<endl;
}
