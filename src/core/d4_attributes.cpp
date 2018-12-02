#include <AttrTable.h>
#include <D4AttributeType.h>
#include <D4Attributes.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sstream>

namespace py = pybind11;

// static inline libdap::D4Attributes::D4AttributesIter get_iter(
//     libdap::D4Attributes& self, const libdap::D4Attribute* dim) {
//   for (auto it = self.dim_begin(); it != self.dim_end(); ++it) {
//     if ((*it)->name() == dim->name()) {
//       return it;
//     }
//   }
//   throw std::invalid_argument(
//       "The requested dimension does not belong to this instance");
// }

void init_d4_attributes(py::module& m) {
  py::enum_<D4AttributeType>(m, "D4AttributeType")
      .value("attr_null_c", D4AttributeType::attr_null_c)
      .value("attr_byte_c", D4AttributeType::attr_byte_c)
      .value("attr_int16_c", D4AttributeType::attr_int16_c)
      .value("attr_uint16_c", D4AttributeType::attr_uint16_c)
      .value("attr_int32_c", D4AttributeType::attr_int32_c)
      .value("attr_uint32_c", D4AttributeType::attr_uint32_c)
      .value("attr_float32_c", D4AttributeType::attr_float32_c)
      .value("attr_float64_c", D4AttributeType::attr_float64_c)
      .value("attr_str_c", D4AttributeType::attr_str_c)
      .value("attr_url_c", D4AttributeType::attr_url_c)
      .value("attr_int8_c", D4AttributeType::attr_int8_c)
      .value("attr_uint8_c", D4AttributeType::attr_uint8_c)
      .value("attr_int64_c", D4AttributeType::attr_int64_c)
      .value("attr_uint64_c", D4AttributeType::attr_uint64_c)
      .value("attr_enum_c", D4AttributeType::attr_enum_c)
      .value("attr_opaque_c", D4AttributeType::attr_opaque_c)
      .value("attr_container_c", D4AttributeType::attr_container_c)
      .value("attr_otherxml_", D4AttributeType::attr_otherxml_c)
      .export_values();

  py::class_<libdap::D4Attribute>(m, "D4Attribute")
      .def(py::init<>())
      .def(py::init<const std::string&, D4AttributeType>(), py::arg("name"),
           py::arg("type"))
      .def("name", &libdap::D4Attribute::name)
      .def("set_name", &libdap::D4Attribute::set_name, py::arg("name"))
      .def("type", &libdap::D4Attribute::type)
      .def("set_type", &libdap::D4Attribute::set_type, py::arg("type"))
      .def("add_value", &libdap::D4Attribute::add_value, py::arg("value"))
      .def("add_value_vector", &libdap::D4Attribute::add_value_vector,
           py::arg("values"))
      .def("__iter__",
           [](libdap::D4Attribute& self) {
             return py::make_iterator(self.value_begin(), self.value_end());
           },
           py::keep_alive<0, 1>())
      .def("num_values", &libdap::D4Attribute::num_values)
      .def("value",
           [](libdap::D4Attribute& self, unsigned int index) -> std::string {
             if (index > self.num_values()) {
               throw std::out_of_range("attribute index out of range");
             }
             return self.value(index);
           },
           py::arg("index"))
      .def("attributes", &libdap::D4Attribute::attributes,
           py::return_value_policy::reference_internal)
      .def("print_dap4", &libdap::D4Attribute::print_dap4, py::arg("xml"))
      .def("dump", [](libdap::D4Attribute& self) -> std::string {
        std::ostringstream ss;
        self.dump(ss);
        return ss.str();
      });

  py::class_<libdap::D4Attributes>(m, "D4Attributes")
      .def(py::init<>())
      .def("transform_to_dap4", &libdap::D4Attributes::transform_to_dap4,
           py::arg("at"))
      .def("get_attrtable", &libdap::D4Attributes::get_AttrTable,
           py::arg("name"), py::return_value_policy::reference_internal)
      .def_static("load_attrtable", &libdap::D4Attributes::load_AttrTable,
                  py::arg("d2_attr_table"), py::arg("d4_attrs"))
      .def("empty", &libdap::D4Attributes::empty)
      .def("add_attribute", &libdap::D4Attributes::add_attribute,
           py::arg("attr"))
      .def("add_attribute_nocopy", &libdap::D4Attributes::add_attribute_nocopy,
           py::arg("attr"), py::keep_alive<1, 2>())
      .def("__iter__",
           [](libdap::D4Attributes& self) {
             return py::make_iterator(
                 self.attribute_begin(), self.attribute_end(),
                 py::return_value_policy::reference_internal);
           },
           py::keep_alive<0, 1>())
      .def("find", &libdap::D4Attributes::find, py::arg("name"),
           py::return_value_policy::reference_internal)
      .def("get", &libdap::D4Attributes::get, py::arg("fqn"),
           py::return_value_policy::reference_internal)
      .def("print_dap4", &libdap::D4Attributes::print_dap4, py::arg("xml"))
      .def("dump", [](libdap::D4Attributes& self) -> std::string {
        std::ostringstream ss;
        self.dump(ss);
        return ss.str();
      });
}