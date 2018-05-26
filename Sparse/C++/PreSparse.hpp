#pragma once
#include <map>
#include <utility>
#include <algorithm>
#include <tuple>
using namespace std;
class PreSparse
{
  map<pair<int,int>,double> M;
  int imax,jmax;
public:
  PreSparse()
  {
    imax=0; jmax=0;
  }
  int size() const {return M.size();}
  void purge()
  {
    M.clear();
  }
  double& operator()(int i,int j)
  {
    imax=max(i,imax);
    jmax=max(j,jmax);
    return M[make_pair(i+1,j+1)];//indices start at 1.
  }
  tuple<int,int,int> sizes() const {return make_tuple(imax+1,jmax+1,size());}
  map<pair<int,int>,double>::const_iterator cbegin()const {return M.cbegin();}
  map<pair<int,int>,double>::const_iterator cend() const {return M.cend();}
};
