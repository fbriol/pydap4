import unittest
import numpy as np
from pydap4 import core


class BaseTypeStack(unittest.TestCase):
    def test_empty(self):
        bt = core.BaseTypeStack()
        self.assertTrue(isinstance(bt, core.BaseTypeStack))
        self.assertTrue(bt.empty())

    def test_fill(self):
        bt = core.BaseTypeStack()
        bt.push(core.Byte("x"))
        bt.push(core.Byte("y"))
        bt.push(core.Byte("z"))
        self.assertFalse(bt.empty())
        self.assertEqual(len(bt), 3)
        self.assertEqual(bt.top().name(), "z")
        bt.pop()
        self.assertEqual(bt.top().name(), "y")


if __name__ == "__main__":
    unittest.main()
