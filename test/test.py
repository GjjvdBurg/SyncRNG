
from __future__ import division

import unittest

from SyncRNG import SyncRNG

class SyncRNGTestCase(unittest.TestCase):

    def test_randi(self):
        s = SyncRNG(seed=123456)
        self.assertEqual(s.randi(), 959852049)
        self.assertEqual(s.randi(), 2314333085)
        self.assertEqual(s.randi(), 2255782734)
        self.assertEqual(s.randi(), 2921461239)
        self.assertEqual(s.randi(), 1024197102)

    def test_rand(self):
        s = SyncRNG(seed=123456)
        self.assertAlmostEqual(s.rand(), 959852049/pow(2, 32))
        self.assertAlmostEqual(s.rand(), 2314333085/pow(2, 32))
        self.assertAlmostEqual(s.rand(), 2255782734/pow(2, 32))
        self.assertAlmostEqual(s.rand(), 2921461239/pow(2, 32))
        self.assertAlmostEqual(s.rand(), 1024197102/pow(2, 32))

    def test_randbelow(self):
        s = SyncRNG(seed=123456)
        self.assertEqual(s.randbelow(5), 4)
        self.assertEqual(s.randbelow(5), 0)
        self.assertEqual(s.randbelow(5), 4)
        self.assertEqual(s.randbelow(5), 4)
        self.assertEqual(s.randbelow(5), 2)

    def test_shuffle(self):
        s = SyncRNG(seed=123456)
        x = [1, 2, 3, 4, 5]
        y = s.shuffle(x)
        self.assertEqual(y, [3, 4, 1, 2, 5])

    def test_first_1000(self):
        s = SyncRNG(seed=0)
        with open("./test/first_1000_seed_0.txt", "r") as fid:
            for line in fid:
                exp = int(line.strip())
                self.assertTrue(exp == s.randi())

if __name__ == '__main__':
    unittest.main()
