import numpy as np

class Weno:
    def __init__(self):
        self.epsilon = 1e-6

    def weno(self, NumFl, Fl, L, In, Out):
        """ Interface reconstruction using WENO scheme. """

        # Built an extenday array with phantom cells to deal with periodicity
        #data = np.concatenate((In[-2:], In, In[0:2]))
        data = np.concatenate((In[-3:], In, In[0:2])) # FIXME: shift indexes so that there is no need to add 3 points at the left side

        # Squared derivatives of the input
        dp1m2p1 = np.square(np.convolve(data, np.flipud([1, -2, 1]), 'valid'))
        dp1m4p3 = np.square(np.convolve(data, np.flipud([1, -4, 3]), 'valid'))
        dp1p0m1 = np.square(np.convolve(data, np.flipud([1, 0, -1]), 'valid'))
        dp3m4p1 = np.square(np.convolve(data, np.flipud([3, -4, 1]), 'valid'))

        # Regularity indicators
        S1 = 13./12. * dp1m2p1 + 1./4. * dp1m4p3
        S2 = 13./12. * dp1m2p1 + 1./4. * dp1p0m1
        S3 = 13./12. * dp1m2p1 + 1./4. * dp3m4p1

        # Unweighted coefficients that can be reused for right reconstruction
        w1_tmp = np.reciprocal(np.square(self.epsilon + S1))
        w2_tmp = np.reciprocal(np.square(self.epsilon + S2))
        w3_tmp = np.reciprocal(np.square(self.epsilon + S3))

        # Weighted coefficients for left reconstruction
        w1 = 1./10. * w1_tmp[0:-3]
        w2 = 3./5. * w2_tmp[1:-2]
        w3 = 3./10. * w3_tmp[2:-1]

        # Coefficients sum
        atot = w1 + w2 + w3

        # Interpolations
        ip2m7p11 = np.convolve(data, np.flipud([2./6., -7./6., 11./6.]), 'valid')
        im1p5p2 = np.convolve(data, np.flipud([-1./6., 5./6., 2./6.]), 'valid')
        ip2p5m1 = np.convolve(data, np.flipud([2./6., 5./6., -1./6.]), 'valid')

        # Left reconstruction
        qm = np.divide(w1 * ip2m7p11[0:-3] + w2 * im1p5p2[1:-2] + w3 * ip2p5m1[2:-1], atot);

        # Note: for the right reconstruction, the input is processed like
        #   its order is reversed: for example, instead of dp1m4p3, we use
        #   dp3m4p1. But in fact, due to some symmetries, the main work has
        #   already be done during the left reconstruction.
        
        # Regularity indicators
        # S1 = 13./12. * dp1m2p1 + 1./4. * dp3m4p1 # == S3 from the first step
        # S2 = 13./12. * dp1m2p1 + 1./4. * dp1p0m1 # == S2 from the first step
        # S3 = 13./12. * dp1m2p1 + 1./4. * dp1m4p3 # == S1 from the first step

        # Weighted coefficients for right reconstruction
        w1 = 1./10. * w3_tmp[3:]
        w2 = 3./5. * w2_tmp[2:-1]
        w3 = 3./10. * w1_tmp[1:-2]

        # Coefficients sum
        atot = w1 + w2 + w3

        # Interpolations
        ip11m7p2 = np.convolve(data, np.flipud([11./6., -7./6., 2./6.]), 'valid')
        # ip2p5m1 = np.convolve(data, np.flipud([2./6., 5./6., -1./6.]), 'valid') # Already calculated
        # im1p5p2 = np.convolve(data, np.flipud([-1./6., 5./6., 2./6.]), 'valid') # Already calculated

        # Right reconstruction
        qp = np.divide(w1 * ip11m7p2[3:] + w2 * ip2p5m1[2:-1] + w3 * im1p5p2[1:-2], atot);

        # Numerical flux
        num_flux = np.empty(len(In)+1)
        num_flux[0:-1] = NumFl.NumFlux(Fl, qm, qp)
        num_flux[-1] = num_flux[0]

        # Derivative
        Out[:] = - (num_flux[1:] - num_flux[:-1]) / (L/len(In))
