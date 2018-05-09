from numba import jitclass,float64,int64
specrando=[
    ("seed",int64), ("a",int64),("c",int64),("m",int64)]
@jitclass(specrando)
class rando:
    def __init__(self):
        self.seed=123456789
        self.a=1103515245
        self.c=12345
        self.m=2**32
    def get(self):
        self.seed= (self.a * self.seed + self.c) % self.m
        return self.seed
    def fv(self,vmax=1.):
        return vmax*float(self.get())/self.m
if __name__ == "__main__":
    R=rando()
    for i in range(0,100):
        print(R.fv(10.))
