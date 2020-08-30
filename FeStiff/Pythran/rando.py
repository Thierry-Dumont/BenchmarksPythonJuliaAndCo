class Rando:
    def __init__(self):
        self.seed = 123456789
        self.a = 1103515245
        self.c = 12345
        self.m = 2 ** 32

    def get(self):
        self.seed = (self.a * self.seed + self.c) % self.m
        return self.seed

    def fv(self, vmax=1.0):
        return vmax * float(self.get()) / self.m


if __name__ == "__main__":
    R = rando()
    for i in range(100):
        print(R.fv(10.0))
