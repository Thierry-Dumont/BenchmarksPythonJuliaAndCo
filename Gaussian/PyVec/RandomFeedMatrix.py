def RandomFeedMatrix(M,R):
    # feed the numpy array M with random values.
    n=M.shape[0]
    m=M.shape[1]
    for i in range(0,n):
        for j in range(0,m):
            M[i,j]= R.fv()
