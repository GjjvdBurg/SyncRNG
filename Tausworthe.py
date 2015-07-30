
import taus

class TauswortheRNG(object):

    def __init__(self, seed=0):
        self.seed = seed
        self.state = taus.seed(seed)

    def randi(self):
        tmp = taus.rand(self.state)
        self.state = tmp[:-1]
        return(tmp[-1])

    def rand(self):
        return self.randi() * 2.3283064365387e-10


if __name__ == '__main__':
    t = TauswortheRNG(112339)
    for i in range(1000000):
        print(t.randi())
