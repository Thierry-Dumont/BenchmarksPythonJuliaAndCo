#include "rando.hpp"
#include <iostream>
#include <algorithm>
#include "factorMatrix.hpp"
#include "RandomFeedMatrix.hpp"
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
int main()
{
  char hostnameC[HOST_NAME_MAX];
  gethostname(hostnameC, HOST_NAME_MAX);
  auto hostname = string(hostnameC);
  cout<<"hostname: "<<hostname<<endl;
  
  map<int,double> times;
  int sizeG=2048;
  int size=sizeG;
  int liminf=2;
  int loop=1; 
    
  Mtime T;
 
  while(size>liminf)
    {
      cout<<size<<endl;
      rando R;
      Array<double,2>  M(size,size+1);
      T.start();
      for(int iter=0;iter<loop;iter++)
        {
            RandomFeedMatrix(M,R);
            factorMatrix(M);
        }
      
      double t=T.sec()/loop;
      times[size]=t;
      size /=2;
      if(size<=128)
	loop*=4;
      else
	loop*=8;
    }
    cout<<"--"<<endl;
    size=sizeG; loop=1;
    while(size>liminf)
    {

        rando R;
        Array<double,2>  M(size,size+1);
        T.start();
        for(int iter=0;iter<loop;iter++)
            RandomFeedMatrix(M,R);
        
        double t=T.sec()/loop;
        times[size]-=t;
        size /=2;
	if(size<=128)
	  loop*=4;
	else
	  loop*=8;
    }
    ofstream f; f.open("../RunningOn"+hostname);
    for(auto e : times)
      f<<e.first<<" "<<e.second<<endl;
    f.close();
    cout<<"end"<<endl;
}
