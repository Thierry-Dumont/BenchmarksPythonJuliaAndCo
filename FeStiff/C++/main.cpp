#include "rando.hpp"
#include "Stiffness.hpp"
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <limits.h>
#include <cmath>
using namespace std;

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
  
  const long int ntri=std::pow(10,8);
  
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
  cout<<ntri<<" triangles."<<endl;
  auto t1=ck();
  for(long int tri=0;tri<ntri;tri++)
    {
      RandomTriangle(R,x,y);
      S(x,y,mat);
    }
  auto T= ck()- -t1;
  cout<<"first phase: "<<static_cast<double>(T)/ CLOCKS_PER_SEC<<" seconds."
      <<endl;
 
  t1= ck();

  for(long int tri=0;tri<ntri;tri++)
    {
      RandomTriangle(R,x,y);
    }
  auto t2= ck()-t1;
  cout<<"second phase: "<<static_cast<double>(t2)/ CLOCKS_PER_SEC <<" seconds"
      <<endl;
 
  T-=t2;
  cout<<"Total time: "<<static_cast<double>(T)/ CLOCKS_PER_SEC <<" seconds."<<
    endl;
  cout<<"Time by triangle: "<<static_cast<double>(T)/ CLOCKS_PER_SEC /ntri<<
    " second."<<endl;
  cout<<endl;
  cout<<"end."<<endl;
}
