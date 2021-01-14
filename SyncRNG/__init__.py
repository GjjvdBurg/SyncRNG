"""
Simple interface to SyncRNG. This file defines a SyncRNG object which can be 
used to seed and pull numbers from the RNG.

"""

from copy import deepcopy
from warnings import warn

from _syncrng import seed as _seed
from _syncrng import rand as _rand


class SyncRNG(object):
    def __init__(self, seed=0):
        self.BPF = 32
        self.seed = seed
        self.state = _seed(self.seed)

    def randi(self):
        tmp = _rand(self.state)
        self.state = tmp[:-1]
        return int(tmp[-1])

    def rand(self):
        return self.randi() * 2.3283064365387e-10

    def randbelow(self, n):
        maxsize = 1 << self.BPF
        if n >= maxsize:
            warn(
                "Underlying random generator does not supply \n"
                "enough bits to choose from a population range this "
                "large.\n"
            )
            return int(self.rand() * n)
        rem = maxsize % n
        limit = (maxsize - rem) / maxsize
        r = self.rand()
        while r >= limit:
            r = self.rand()
        return int(r * maxsize) % n

    def shuffle(self, x):
        y = deepcopy(x)
        for i in reversed(range(1, len(y))):
            j = self.randbelow(i + 1)
            y[i], y[j] = y[j], y[i]
        return y
