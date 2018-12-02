import unittest
from pydap4 import core


class AttrTable(unittest.TestCase):
    def test_load_attr_table(self):
        at = core.AttrTable()
        at.append_attr("month", "String", "Feb")
        at.append_attr("month", "String", "Feb")

        at.append_attr("month_a", "String", "Jan")
        at.append_attr("month_a", "String", "Feb")
        at.append_attr("month_a", "String", "Mar")

        at.append_attr("Date", "Int32", "12345")
        at.append_attr("day", "Int32", "01")
        at.append_attr("Time", "Float64", "3.1415")

        def dump():
            build = ''
            for item in at:
                line = item.name + " " + str(item.type) + " "
                for attr in item.attr:
                    line += attr + " "
                build += line.strip() + "\n"
            return build

        self.assertEqual("""month AttrType.Attr_string Feb Feb
month_a AttrType.Attr_string Jan Feb Mar
Date AttrType.Attr_int32 12345
day AttrType.Attr_int32 01
Time AttrType.Attr_float64 3.1415
""", dump())

        self.assertEqual(at.get_attr_type("month"), core.AttrType.Attr_string)
        self.assertEqual(at.get_attr_num("month"), 2)
        for idx in range(at.get_attr_num("month")):
            self.assertEqual(at.get_attr("month", idx), "Feb")

        self.assertEqual(at.get_attr("month"), "Feb")
        at.del_attr("month")
        self.assertEqual(at.get_attr("month"), "")
        self.assertEqual(at.get_attr_num("month"), 0)
        self.assertEqual("""month_a AttrType.Attr_string Jan Feb Mar
Date AttrType.Attr_int32 12345
day AttrType.Attr_int32 01
Time AttrType.Attr_float64 3.1415
""", dump())
        self.assertEqual(at.get_attr_vector("month_a"),
                         ["Jan", "Feb", "Mar"])

        writer = core.XMLWriter()
        at.print_xml_writer(writer)
        self.assertEqual(
            writer.get_doc(),
            """<?xml version="1.0" encoding="ISO-8859-1"?>
<Attribute name="month_a" type="String">
    <value>Jan</value>
    <value>Feb</value>
    <value>Mar</value>
</Attribute>
<Attribute name="Date" type="Int32">
    <value>12345</value>
</Attribute>
<Attribute name="day" type="Int32">
    <value>01</value>
</Attribute>
<Attribute name="Time" type="Float64">
    <value>3.1415</value>
</Attribute>
""")
        writer = core.XMLWriter()
        at.print_dap4(writer)
        self.assertEqual(
            writer.get_doc(),
            """<?xml version="1.0" encoding="ISO-8859-1"?>
<Attribute name="month_a" type="String">
    <value>Jan</value>
    <value>Feb</value>
    <value>Mar</value>
</Attribute>
<Attribute name="Date" type="Int32">
    <value>12345</value>
</Attribute>
<Attribute name="day" type="Int32">
    <value>01</value>
</Attribute>
<Attribute name="Time" type="Float64">
    <value>3.1415</value>
</Attribute>
""")

    def test_container(self):
        at = core.AttrTable()
        at.append_attr("month", "String", "Jan")
        at.append_attr("month", "String", "Feb")
        at.append_attr("month", "String", "Mar")
        self.assertTrue(at.simple_find("month") is not None)
        self.assertTrue(at.simple_find("day") is None)
        self.assertEqual(at.simple_find("month").type,
                         core.AttrType.Attr_string)
        self.assertEqual(at.simple_find("month").name,
                         "month")
        day = at.append_container("container")
        day.append_attr("day", "String", "Mon")
        day.append_attr("day", "String", "Tue")
        day.append_attr("day", "String", "Wed")
        it = at.find_container("container")
        self.assertEqual(id(it), id(day))
        date = day.append_container("calendar")
        date.append_attr("day", "Int32", "1")
        date.append_attr("day", "Int32", "2")
        date.append_attr("day", "Int32", "3")
        it = at.find_container("calendar")
        self.assertTrue(it is None)
        it = day.find_container("calendar")
        self.assertEqual(id(it), id(date))
        self.assertEqual(id(day.simple_find_container("calendar")), id(it))
        at.add_container_alias("cal1", date)
        at.add_value_alias(date, "cal2", "calendar")
        self.assertEqual(at.find_container("cal1").get_attr_vector("day"),
                         ['1', '2', '3'])
        self.assertEqual(at.find_container("cal2").get_attr_vector("day"),
                         ['1', '2', '3'])


if __name__ == "__main__":
    unittest.main()
