import unittest
import numpy as np
import pydap4.core as core
import pydap4.exceptions as exceptions


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


class BaseType(unittest.TestCase):
    def test(self):
        x = core.Byte("x")
        self.assertFalse(x.is_dap4())
        x.set_is_dap4(True)
        self.assertTrue(x.is_dap4())
        self.assertEqual(x.name(), "x")
        x.set_name("X")
        self.assertEqual(x.name(), "X")
        self.assertEqual(x.FQN(), "X")
        self.assertEqual(x.type(), core.Type.dods_byte_c)
        x.set_type(core.Type.dods_int16_c)
        self.assertEqual(x.type(), core.Type.dods_int16_c)
        self.assertEqual(x.dataset(), "")
        self.assertEqual(x.length(), 1)
        x.set_length(2)
        self.assertTrue(x.is_simple_type())
        self.assertFalse(x.is_vector_type())
        self.assertFalse(x.is_constructor_type())
        self.assertFalse(x.synthesized_p())
        x.set_synthesized_p(True)
        self.assertTrue(x.synthesized_p())
        self.assertEqual(x.element_count(), 1)
        self.assertFalse(x.read_p())
        x.set_read_p(True)
        self.assertFalse(x.send_p())
        x.set_send_p(True)
        self.assertTrue(len(list(x.get_attr_table())) == 0)
        at = core.AttrTable()
        at.append_attr("month", "String", "Jan")
        x.set_attr_table(at)
        del at
        self.assertTrue(len(list(x.get_attr_table())) == 1)
        self.assertFalse(x.is_in_selection())
        x.set_in_selection(True)
        self.assertTrue(x.is_in_selection())
        self.assertIsNone(x.get_parent())
        parent = core.Array("x", core.Byte("x"))
        id_parent = id(parent)
        x.set_parent(parent)
        del parent
        self.assertEqual(id(x.get_parent()), id_parent)
        at = core.AttrTable()
        at.append_attr("month", "String", "Feb")
        x.transfer_attributes(at)
        bts = core.BaseTypeStack()
        var = core.Byte("x")
        parent = core.Array("x", var)
        rc = parent.var("x", bts)
        self.assertNotEqual(id(var), id(rc))
        self.assertEqual(id(rc.get_parent()), id(parent))
        self.assertEqual(id(bts.top()), id(parent))
        x.set_value(128)
        with self.assertRaises((exceptions.Error,)):
            x.read()
        self.assertTrue(x.check_semantics("msg"))
        with self.assertRaises((exceptions.Error,)):
            x.ops(var, 3)
        with self.assertRaises((exceptions.Error,)):
            x.d4_ops(var, 3)
        self.assertEqual(x.width(), 1)
        x.print_xml()
        x.print_xml_writer(core.XMLWriter())
        x.print_dap4(core.XMLWriter())
        with self.assertRaises((exceptions.Error,)):
            x.intern_data()


if __name__ == "__main__":
    unittest.main()
