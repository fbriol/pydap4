import unittest
import numpy as np
from pydap4 import core


class Crc32(unittest.TestCase):
    def test(self):
        crc = core.Crc32()
        self.assertTrue(isinstance(crc, core.Crc32))
        crc.add_data(np.arange(1, 256, dtype="uint8"))
        self.assertEqual(crc.get_crc32(), 3491110791)
        crc.reset()
        self.assertEqual(crc.get_crc32(), 0)


if __name__ == "__main__":
    unittest.main()
