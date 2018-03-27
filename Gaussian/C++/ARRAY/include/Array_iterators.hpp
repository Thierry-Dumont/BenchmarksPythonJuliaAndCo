#ifndef Array_iterators__h
#define Array_iterators__h
////////////////////////////////////////////////////////////////////////
/// Iterator class for the Array class
///   should be compatible with stl iterators.
/// \brief iterator
///////////////////////////////////////////////////////////////////////
#include <iterator>
template<class T> class ArrayIterator : 
  public std::iterator<std::input_iterator_tag, T>
{
  T* p;
public:
  //! constructor
  //! \param x C vector of an Array.
  ArrayIterator(T* x) :p(x) {}
  //! copy constructor.
  //! \param mit
  ArrayIterator(const ArrayIterator& mit) :  p(mit.p) {}
  //! pre-instantiation (++I)
  ArrayIterator& operator++() {++p;return *this;}
  //! post-instantiation (I++)
  ArrayIterator operator++(int) {ArrayIterator tmp(*this); 
    ++p; return tmp;}
  //! equality test
  //! \param rhs test *this with rhs.
  bool operator==(const ArrayIterator& rhs) {return p==rhs.p;}
  //! non equality test
  //! \param rhs test *this with rhs.
  bool operator!=(const ArrayIterator& rhs) {return p!=rhs.p;}
  //! dereferentiation.
  T& operator*() {return *p;}
  //! return the pointer.
  T* Tpointer() {return p;}
};
#endif
