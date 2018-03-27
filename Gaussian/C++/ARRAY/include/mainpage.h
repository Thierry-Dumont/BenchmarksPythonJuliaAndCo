/*! \mainpage Documentation.

\section Introduction

Array is a template class of... arrays with the following properties:

- Any type of data can be stored (but the data type must provide the arithmetic operations
  + - * / and -may be- other features). 

Actually we have in mind to store int, float
  double, long int and so on, but this not limitative.

- Dimensions are limited to 4.

- The indexation can be row major (like in C) or column major (like in Fortran)

- Indexes can be in any relative integer interval, for any dimension.  

\section namespace

  Array objects belong to the namespace: Arrays.

\section Warning
   
  Array class uses a macro defined in "MacroRestrict.hpp". You must
  define GCC in your compiler options if you use g++ or ICC if you use
  the Intel icc compiler, or CLANG if you use clang++
. If you use an other compiler, you must adapt
  the macro. The macro defines how to treat the "Restrict" keyword which 
  defines restricted pointers.

\section Instantiation

- Doing something like: 

   Array<double,3> C(Range(-3,4),100,Range(20,100))

   will create an Array C, with a C-like indexation. First index will be
   in [-3,4[, second index in [0,100[ and the third one in [20,100[.

- Doing:

   Array<double,3,FEngine> C(Range(-3,4),100,Range(20,100))

   will create an Array F, with a Fortran-like indexation. First index will be
   in [-3,4], second index in [1,100] and the third one in [20,100].

 - Note that the index bounds (defined by Range()) do not behave the same way 
   in C-like Array and in Fortran-like Array! 


\section Methods

- Operations between arrays:

 =, += , -= 

 Example:

 Array<double,3,FEngine> C((Range(-3,4),100,Range(20,100)),D(Range(-3,4),100,Range(20,100))

 C+=D

 But both operands must share the same pattern (same bounds of indices, same indexation). 

- Operations with scalars: =, += , -=, *= /=

Example:  C+=1; D=0;

- Indexation:

Any n dimensional Array has the operator (i,...in); example:

s=A(i,j)  ; A(i,j)=1.;

- Getting the bounds of an index:

A.mins(i) and A.maxs(i) return the lower and upper bound of index i.

\section Iterator

Array objects define an iterator; example:

Array<double,3> C(Range(-3,4),100,Range(20,100))

for(auto  I=C.begin();I!=C.end();I++)
     \\*I=4;

\section Data

Method data() returns a pointer to the begining of the datas of an Array. Note that the storage is continuous in an Array.

\section cycle

For time steping schemes, it is often very useful to permute the content of 
Arrays. Given Array's A,B,C:

A.cycle(B) permutes the datas of A and B.

A.cycle(B,C) make a circular permutation of contents of A, B and C.

These are  permutation of datas, not a copy ! 

\section Slices

For C-like and Fortran-like Array's, we define "natural" slices: that is to
say slices along the fastest varying coordinate: 

for example, with a  2 dimensional C-like 
Array, we can get slices which are lines, and with Fortran like Arrays, slices
wich are columns.

A slice is a (n-1) dimensional vue of a n dimensional Array, along the 
last coordinate for Fortran-like Arrays, and along the first one with C-like
Arrays. 

NO DATA IS COPIED! We only build a vue. 

Note that it is possible to chain slices for example to get a 1 dimensional 
Array from a 4 dimensional one.

\section Apply_a_function

You can apply a function to a whole array. Example:

Array<double,3,3> X;
X.applyFonc<sin>();

You can use any function of yours, but it must be compatible with the model:

T function(T x)

where T is the type of what is stored in the Array.

\section Adoption

Let *_x be a C vector. We can create an Array A which "adopts" _x: that is to say 
in the new Array created,

 we will have x= _x.

In this case, x (thus xc) will not be deleted when A will be deleted.

Constructors are:

 Array(Range R0,T *_x) in 1d

 Array(Range R0,Range R1,T *_x) in 2d

and so on.

The range objects R0, R1,... must decribe an object of the same size as xc.

So, these constructors are definitively not safe! (this is for adults
 programmers only :-) ).

\section Resizing

Resizing an Array with A.resize( ) is possible (arguments are Range objects).

All datas are lost! 

\section DEBUG

If DEBUG is defined, Array perform bounds check for indices (and other verifications at run time). In this case, the code will be slow!

*/
