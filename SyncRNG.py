"""
Simple interface to SyncRNG. This file defines a SyncRNG object which can be 
used to seed and pull numbers from the RNG.

"""

import syncrng

class SyncRNG(object):

    def __init__(self, seed=0):
        self.seed = seed
        self.state = syncrng.seed(seed)

    def randi(self):
        tmp = syncrng.rand(self.state)
        self.state = tmp[:-1]
        return(tmp[-1])

    def rand(self):
        return self.randi() * 2.3283064365387e-10
