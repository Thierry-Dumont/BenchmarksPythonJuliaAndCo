def random_triangle(R, x, y):
    # domain is [0,10]x[0,10]
    for i in range(3):
        x[i] = R.fv(10.0)
    for i in range(3):
        y[i] = R.fv(10.0)
