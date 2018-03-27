//#define DEBUG
#include "Array.hpp"
#include "Array_Output.hpp"
#include <iostream>
#include <algorithm>
#include <random>
using namespace std;
template< class T> void print(T x) {cout<<x<<" ";}//see foreach above.
double my_fonc(double x)
{
  return 2.*x;
}
int main()
{
  using namespace Arrays;
  cout<<endl<<endl<<"============= Testing ================"<<endl<<endl;

  cout<<"C arrays:"<<endl;
  cout<<endl<<"declare arrays of double...";
  // declare 2 Arrays and initialize the first one to 1.0, the second one
  // to 2.
  Array<double,2> X(10,10),X1(10,10);
  cout<<"put a constant in arrays...";
  X=1.0; X1=2.0;
  
  cout<<endl<<"test for operator <<"<<endl<<endl;
  cout<<X<<endl;

  cout<<"C arrays with various Ranges...";
  // an other array:
  Array<double,2> Y(Range(-5,2),Range(-1,5));
  
  cout<<endl<<" Test indexation operator...";
  // use of operator( )
  for(int i=Y.mins(0);i<Y.maxs(0);i++)
    for(int j=Y.mins(1);j<Y.maxs(1);j++)
      Y(i,j)=i+j;

  cout<<endl<<"Multiplication by a constant...";
  // multiplication
  Y*=100.;

  //printing, but we do not use the << overloaded for Arrays:
  for(int i=Y.mins(0);i<Y.maxs(0);i++)
    {
      cout<<i<<" ";
      for(int j=Y.mins(1);j<Y.maxs(1);j++)
	cout<<Y(i,j)<<" ";
      cout<<endl;
    }

  cout<<endl<<"Test copy constructor...";
  // copy constructor
  Array<double,2> Z(X);
  // // ... again:
  Array<double,2> T=X;
  
  cout<<endl<<"Test some operations between arrays...";
  // // some operations:
  X1=X;
  X+=X;

  
  //Try something illegal in DEBUG mode ; if DEBUG is not set, this can
  // result in a memory fault (uncomment to following lines for testing).
  //cout<<"In debug mode, we now get an exception (caught!)"<<endl;
  // try
  //   {
  //     X(100,1000)=1.0;
  //   }
  // catch(ArrayException)
  //   {
  //     cout<<"Exception caught!"<<endl<<endl;
  //   }

  cout<<endl<<"Experiment with Fortran like Arrays...";

  Array<double,2,FEngine> XF(10,10),X1F(10,10);
  XF=1.0;
  Array<double,2,FEngine> YF(Range(-1,2),Range(-1,2));
  Array<double,2,FEngine> ZF(XF);
  Array<double,2,FEngine> TF=XF;
 
  X1F=XF;
  XF+=XF;
  cout<<endl<<"XF(4,5)= "<<XF(4,5)<<endl;
  XF(4,5)=-1.;
  cout<<"XF(4,5)= "<<XF<<endl;

  XF=1.;
  cout<<endl<<"Test iterator on Fortran like array..."<<endl;;
  for(auto I=XF.begin();I!=XF.end();I++)
    cout<<*I<<" "; cout<<endl;
  
  cout<<endl<<"Test operator <<:..."<<endl;
  cout<<XF<<endl;
 
  for(int i=1;i<=10;i++)
    for(int j=1;j<=10;j++)
      XF(i,j)=i+j;
  cout<<XF<<endl;

  // All the Arrays where 2d ones; we now do the same sort of operations
  // with 3d and 4d Arrays.
  cout<<endl<<"Some operations on 3d Arrays...";
  Array<double,3> X3(100,100,100);
  Array<double,3,FEngine> X3F(100,100,100);

  cout<<endl<<"Same, onn 4d Arrays...";
  Array<double,4> X4(100,100,100,100);
  Array<double,4,FEngine> X4F(100,100,100,100);

  // // Use of iterators.
  cout<<endl<<"Using iterators..."<<endl;

  cout<<endl<<"Copy a Fortran indexed array into a C one using iterator..."<<
    "(Do not do this!)...";
  // this is a way to copy a Fortran indexed array into a C indexed one
  // You should never do this, performances are low !
  Array<double,3,FEngine>::iterator J=X3F.begin();
  for(auto I=X3.begin();I!=X3.end();I++)
    {*J=*I; ++J;}

  cout<<endl<<"Our iterator should be STL compatible...";
  // Our iterator should be STL compatible:
  cout<<"using for_each:"<<endl;
  for_each(X.begin(),X.end(),print<Array<double,2>::value_type>);
  cout<<endl;


  // Mixing types:
  cout<<endl<<"Mixing types...";
   Array<int,3> X3I(100,100,100);
   X3I=1;
   X3=X3I;
   X3+=X3I;
   X3-=X3I;
   X3=X3I;
   Array<char,3> CX(100,100,100);
   X3I=CX;

   Array<int*,3> Xp(100,100,100);
   // Of course this is not correct (uncomment and compile!):
   //X3I=Xp;
   
   // content permutation
   cout<<endl<<"content permutation between 2 and 3 arrays...";
   XF.cycle(X1F);
   TF.cycle(XF,X1F);

   //-- resizing:
   cout<<endl<<"resizing"<<endl;
   Array<int,3> XR;
   XR.resize(20,12,10);
   

   //--- slicing
   cout<<"slicing:"<<endl;
   XR.resize(4,5,6);
   for(int i=0;i<4;i++)
     for(int j=0;j<5;j++)
       for(int k=0;k<6;k++)
	 XR(i,j,k)=i+j+k;
   Array<int,2> Sl= XR.slice_ref(3);
   cout<<Sl<<endl;

   
   Array<int,2> YR(4,5);
   for(int i=0;i<4;i++)
     for(int j=0;j<5;j++)
       YR(i,j)=i+j;
   cout<<"YR:"<<endl;
   cout<<YR<<endl;

   cout<<"All the natural slices of YR: "<<endl;
   for(int i=0;i<4;i++)
     {
       cout<<"slice "<<i<<endl<< YR.slice_ref(i)<<endl;
       cout<<"--"<<endl;
     }

   Array<int,2,FEngine> YRF(4,4);
   for(int i=1;i<=4;i++)
     for(int j=1;j<=4;j++)
       YRF(i,j)=i+j;
   cout<<"YRF:"<<endl;
   cout<<YRF<<endl;

   cout<<"All the natural slices of the Fortran Array YRF: "<<endl;
   for(int i=1;i<=4;i++)
     cout<<"slice "<<i<<endl<< YRF.slice_ref(i)<<endl;
   
   Array<int,2> YR1(Range(-1,4),3);
   for(int i=-1;i<4;i++)
     for(int j=0;j<3;j++)
       YR1(i,j)=i+j;
   cout<<"YR1:"<<endl<<YR1<<endl;
   cout<<"All the natural slices of YR1: "<<endl;
   for(int i=-1;i<4;i++)
     {
       cout<<"slice "<<i<<endl<< YR1.slice_ref(i)<<endl;
       cout<<"--"<<endl;
     }

   Array<int,2,FEngine> YRF1(4,Range(-1,4));
   for(int i=1;i<=4;i++)
     for(int j=-1;j<=4;j++)
       YRF1(i,j)=i+j;
   cout<<"YRF1:"<<endl;
   cout<<YRF1<<endl;

   cout<<"All the natural slices of the Fortran Array YRF1: "<<endl;
   for(int i=-1;i<=4;i++)
     cout<<"slice "<<i<<endl<< YRF1.slice_ref(i)<<endl;
   
   Array<int,2,FEngine> YRF2(Range(-1,4),4);
   for(int i=-1;i<=4;i++)
     for(int j=1;j<=4;j++)
       YRF2(i,j)=i+j;
   cout<<"YRF2:"<<endl;
   cout<<YRF2<<endl;

   cout<<"All the natural slices of the Fortran Array YRF2: "<<endl;
   for(int i=1;i<=4;i++)
     cout<<"slice "<<i<<endl<< YRF2.slice_ref(i)<<endl;
    
   // chain slices!
   random_device rd;
   mt19937 gen(rd());
   uniform_int_distribution<> rai(1,100);
   Array<int,3> SX(4,5,6);
   for(auto I=SX.begin();I!=SX.end();I++)
     *I=rai(gen);
   cout<<endl<<"chain slices:"<<endl;

   cout<<SX.slice_ref(2)<<endl;
   cout<<"-----"<<endl;
   cout<<SX.slice_ref(2).slice_ref(1)<<endl;
   cout<<"-----"<<endl;
   
   cout<<endl<<"reference:"<<endl;
   Array<int,2,FEngine> YRF3;
   YRF3.reference(YRF2);

   cout<<YRF2<<endl<<endl<<YRF3<<endl;

   //apply a function to a random array:

   Array<double,2> XFF(5,5);
   uniform_real_distribution<double> rad(0.,4.*atan(1.));

   for(auto I=XFF.begin();I!=XFF.end();I++)
     *I=rad(gen);

   
   cout<<"XFF:"<<endl<<XFF<<endl;

   XFF.applyFonc<sin>();

   cout<<"The transformed XFF array:"<<endl<<XFF<<endl;

   XFF.applyFonc<my_fonc>();

   cout<<"XFF, transformed by my_fonc:"<<endl<<XFF<<endl;

   cout<<endl<<endl<<"=== end ==="<<endl;
}
