
from SyncRNG import SyncRNG

def test_randi():
    s = SyncRNG(seed=123456)
    for i in range(5):
        print(s.randi())

def test_rand():
    s = SyncRNG(seed=123456)
    for i in range(5):
        print(s.rand())

def test_randbelow():
    s = SyncRNG(seed=123456)
    for i in range(5):
        print(s.randbelow(i+1))

def test_shuffle():
    s = SyncRNG(seed=123456)
    x = [1, 2, 3, 4, 5]
    for i in range(5):
        y = s.shuffle(x)
        x = y
        print(y)

def main():
    test_randi()
    test_rand()
    test_randbelow()
    test_shuffle()

if __name__ == '__main__':
    main()
