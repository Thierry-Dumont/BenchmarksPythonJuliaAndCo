from math import exp,sin,cos
#pythran export capsule implicit(float)
def implicit(t):
    # implicit = root of  4*sin(x)-exp(x)+t
    # Newton iterations, starting from zero:    
    x=0.0
    F= 4*sin(x)-exp(x)+t
    while abs(F)> 1.e-15:
        x-= F/(4*cos(x) - exp(x))
        F=  4*sin(x)-exp(x)+t
    return x
