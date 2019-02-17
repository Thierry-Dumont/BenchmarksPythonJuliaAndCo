#pythran export trapz(float(float),float,float,int32)
def trapz(F,a,b,n):
    h=(b-a)/n
    sum=0.5*(F(a)+F(b))
    for i in range(1,n):
        sum+=F(i*h)
    return sum*h
