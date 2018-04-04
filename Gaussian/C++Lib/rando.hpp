#pragma once
class rando{
    const long int a,c,m;
    long int  seed;
    public:
    rando():a(1103515245),c(12345),m(4294967296)
    {
        seed=123456789;
    }
    long int get(){
        seed= (a * seed + c) % m;
        return seed;
    }
    double fv(double vmax=1.)
    {
        return vmax*(double) get()/m;
    }
};

