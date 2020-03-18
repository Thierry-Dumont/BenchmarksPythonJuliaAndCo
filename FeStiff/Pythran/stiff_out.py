
# pythran export stiff_out(float[:],float[:],float[:],float[:, :, :],float[:, :, :])
def stiff_out(x, y, out, grads, gq):
    a11 = -y[0] + y[2]
    a12 = y[0] - y[1]
    a21 = x[0] - x[2]
    a22 = -x[0] + x[1]

    for f in range(6):
        for p in range(3):
            grads[f, p, 0] = a11 * gq[f, p, 0] + a12 * gq[f, p, 1]
            grads[f, p, 1] = a21 * gq[f, p, 0] + a22 * gq[f, p, 1]

    det = -(x[1] - x[2]) * y[0] + (x[0] - x[2]) * y[1] - (x[0] - x[1]) * y[2]
    dv = 1.0 / (6.0 * det)
    ii = 0
    for i in range(6):
        for j in range(i + 1):
            s = 0.0
            for k in range(3):
                s += (
                    grads[i, k, 0] * grads[j, k, 0]
                    + grads[i, k, 1] * grads[j, k, 1]
                )
            out[ii] = dv * s
            ii += 1
