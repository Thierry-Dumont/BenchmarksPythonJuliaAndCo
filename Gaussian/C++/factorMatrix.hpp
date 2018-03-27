#pragma once
#include "Array.hpp"
#include <cmath>
#include <iostream>
using namespace Arrays;
void factorMatrix(Array<double,2>& M)
{
    const int n=M.maxs(0),m=M.maxs(1);
    for(int line=0;line<n-1;line++)
    {
        //find larger coefficient (absolute value) in remaining line-th column:
        int cmax=line;
        double vmax= std::abs(M(line,line));
        for(int i=line+1;i<n;i++)
        {
            if(abs(M(i,line)) > vmax)
            {
                vmax= abs(M(i,line));
                cmax= i;
            }
        }
        if(cmax != line)
        {
            //permutate:
            for(int j=line;j<m;j++)
            {
                double t= M(line,j);
                M(line,j)= M(cmax,j);
                M(cmax,j)= t;
            }
        }
        //eliminate
        double pivot=M(line,line);
        for(int j=line+1;j<n;j++)
        {
            double v= M(j,line)/pivot;
            for(int k=line;k<m;k++)
                M(j,k)-= v*M(line,k);
        }

    }
}


