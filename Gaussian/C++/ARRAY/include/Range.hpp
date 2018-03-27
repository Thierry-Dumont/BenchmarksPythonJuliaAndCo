#ifndef Range__h
#define Range__h
namespace Arrays {
  ////////////////////////////////////////////////////////////////
  /// This classe is a model of integer intervals.
  ///
  ///\brief class of integer intervals.
  ///////////////////////////////////////////////////////////////
struct Range
{
  int begin,end;
  bool one;// this means: created with one argument.
public:
  //! constructor
  //! \note should not be used by end user!
  Range()
  {
  }
  //! constructor
  //! \param i
  //! \param j
  Range(int i,int j): begin(i),end(j),one(false){}
  //! constructor; creates [0,i[
  //! \param i
  Range(int i): begin(0),end(i),one(true){}
  //! copy constructor
  //! \param R
  Range(const Range& R)
  {
    begin=R.begin; end=R.end; 
    one=R.one;
  }
  //! operator =
  //! \param R
  void operator=(const Range& R)
  {
    begin=R.begin; end=R.end; 
    one=R.one;    
  }
  //! destructor.
  ~Range(){}
};
}

#endif
