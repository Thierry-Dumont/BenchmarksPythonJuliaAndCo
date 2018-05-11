import numpy as np
import Burghers as Burg
import GodunovFlux as Godunov


f=lambda x,y: Godunov.NumFlux(Burg.minf,Burg.maxf,x,y)



p(f,1.,2.)
