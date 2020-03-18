import numpy as np

from stiff_out import stiff_out


class Stiffness:
    __slots__ = ["grads", "gq"]

    def __init__(self):
        self.grads = np.empty((6, 3, 2))

        # fmt: off
        gq = np.array(
            [-1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 0.0, 1.0, 0.0, -1.0,
            0.0, 0.0, -1.0, 0.0, 1.0, 0.0, 1.0, 0.0, -2.0, -2.0, -2.0,
            2.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0, 2.0, -2.0, -2.0,
            -2.0, 0.0]
        )
        # fmt: on

        self.gq = gq.reshape(self.grads.shape)

    def op(self, x, y, m):
        stiff_out(x, y, m, self.grads, self.gq)
