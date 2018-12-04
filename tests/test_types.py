import math
import unittest
import pydap4.core as core


class BaseTestType(unittest.TestCase):
    def __init__(self, classname, *args, **kwargs):
        super(BaseTestType, self).__init__(*args, **kwargs)
        self.classname = classname

    def _test(self, value, signed=False, places=0):
        class_ = getattr(core, self.classname)
        instance = class_("x")
        self.assertTrue(isinstance(instance, class_))
        instance = class_("x", "d")
        self.assertTrue(isinstance(instance, class_))
        self.assertTrue(isinstance(instance.width(), int))
        self.assertNotEqual(id(instance), id(instance.ptr_duplicate()))
        crc32 = core.Crc32()
        instance.compute_checksum(crc32)
        self.assertTrue(isinstance(crc32.get_crc32(), int))
        self.assertEqual(instance.value(), 0)
        instance.set_value(value)
        if places:
            self.assertAlmostEqual(instance.value(), value, places=5)
            self.assertEqual(
                instance.print_val(),
                "%s x = %s;\n" % (
                    self.classname, str(value)[:places + 1]))
        else:
            self.assertEqual(instance.value(), value)
            self.assertEqual(
                instance.print_val(),
                "%s x = %d;\n" % (self.classname, value))

        self.assertTrue(isinstance(instance.dump(), str))
        if signed:
            instance.set_value(-value)
            if places:
                self.assertAlmostEqual(instance.value(), -value, places=5)
            else:
                self.assertEqual(instance.value(), -value)


class TestByte(BaseTestType):
    def __init__(self, *args, **kwargs):
        super(TestByte, self).__init__("Byte", *args, **kwargs)

    def test(self):
        self._test(128)


class TestInt8(BaseTestType):
    def __init__(self, *args, **kwargs):
        super(TestInt8, self).__init__("Int8", *args, **kwargs)

    def test(self):
        self._test(127, True)


class TestInt16(BaseTestType):
    def __init__(self, *args, **kwargs):
        super(TestInt16, self).__init__("Int16", *args, **kwargs)

    def test(self):
        self._test(32767, True)


class TestUInt16(BaseTestType):
    def __init__(self, *args, **kwargs):
        super(TestUInt16, self).__init__("UInt16", *args, **kwargs)

    def test(self):
        self._test(2**(16) - 1, False)


class TestInt32(BaseTestType):
    def __init__(self, *args, **kwargs):
        super(TestInt32, self).__init__("Int32", *args, **kwargs)

    def test(self):
        self._test(2**(32 - 1) - 1, True)


class TestUInt32(BaseTestType):
    def __init__(self, *args, **kwargs):
        super(TestUInt32, self).__init__("UInt32", *args, **kwargs)

    def test(self):
        self._test(2**(32) - 1, False)


class TestInt64(BaseTestType):
    def __init__(self, *args, **kwargs):
        super(TestInt64, self).__init__("Int64", *args, **kwargs)

    def test(self):
        self._test(2**(64 - 1) - 1, True)


class TestUInt64(BaseTestType):
    def __init__(self, *args, **kwargs):
        super(TestUInt64, self).__init__("UInt64", *args, **kwargs)

    def test(self):
        self._test(2**(64) - 1, False)


class TestFloat32(BaseTestType):
    def __init__(self, *args, **kwargs):
        super(TestFloat32, self).__init__("Float32", *args, **kwargs)

    def test(self):
        self._test(math.pi, True, 6)


class TestFloat64(BaseTestType):
    def __init__(self, *args, **kwargs):
        super(TestFloat64, self).__init__("Float64", *args, **kwargs)

    def test(self):
        self._test(math.pi, True, 15)


if __name__ == "__main__":
    unittest.main()
