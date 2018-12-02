#include <AttrTable.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sstream>
#include "optional.hpp"

namespace py = pybind11;

static libdap::AttrTable* recursive_find(libdap::AttrTable* self,
                                         const std::string& name) {
  auto it = self->attr_begin();
  while (it != self->attr_end()) {
    if (name == (*it)->name) {
      return self;
    } else if ((*it)->type == libdap::Attr_container) {
      auto at = recursive_find((*it)->attributes, name);
      if (at != nullptr) {
        return at;
      }
    }
  }
  return nullptr;
}

void init_attr_table(py::module& m) {
  py::enum_<libdap::AttrType>(m, "AttrType")
      .value("Attr_unknown", libdap::AttrType::Attr_unknown)
      .value("Attr_container", libdap::AttrType::Attr_container)
      .value("Attr_byte", libdap::AttrType::Attr_byte)
      .value("Attr_int16", libdap::AttrType::Attr_int16)
      .value("Attr_uint16", libdap::AttrType::Attr_uint16)
      .value("Attr_int32", libdap::AttrType::Attr_int32)
      .value("Attr_uint32", libdap::AttrType::Attr_uint32)
      .value("Attr_float32", libdap::AttrType::Attr_float32)
      .value("Attr_float64", libdap::AttrType::Attr_float64)
      .value("Attr_string", libdap::AttrType::Attr_string)
      .value("Attr_url", libdap::AttrType::Attr_url)
      .value("Attr_other_xml", libdap::AttrType::Attr_other_xml)
      .export_values();

  py::class_<libdap::AttrTable> attr_table(m, "AttrTable");

  py::class_<libdap::AttrTable::entry>(attr_table, "entry")
      .def(py::init<>())
      .def_readwrite("name", &libdap::AttrTable::entry::name)
      .def_readwrite("type", &libdap::AttrTable::entry::type)
      .def_readwrite("is_alias", &libdap::AttrTable::entry::is_alias)
      .def_readwrite("aliased_to", &libdap::AttrTable::entry::aliased_to)
      .def_readwrite("is_global", &libdap::AttrTable::entry::is_global)
      .def_readwrite("attributes", &libdap::AttrTable::entry::attributes,
                     py::return_value_policy::reference_internal)
      .def_readwrite("attr", &libdap::AttrTable::entry::attr,
                     py::return_value_policy::reference_internal);

  attr_table.def(py::init<>())
      .def("erase", &libdap::AttrTable::erase)
      .def("get_size", &libdap::AttrTable::get_size)
      .def("get_name",
           [](const libdap::AttrTable& self) -> std::string {
             return self.get_name();
           })
      .def("set_name", &libdap::AttrTable::set_name, py::arg("name"))
      .def("get_parent", &libdap::AttrTable::get_parent,
           py::return_value_policy::reference_internal)
      .def("is_global_attribute",
           [](const libdap::AttrTable& self) -> bool {
             return self.is_global_attribute();
           })
      .def("set_is_global_attribute",
           [](libdap::AttrTable& self, bool ga) -> void {
             self.set_is_global_attribute(ga);
           },
           py::arg("ga"))
      .def("append_attr",
           [](libdap::AttrTable& self, const string& name, const string& type,
              const string& value) -> unsigned int {
             return self.append_attr(name, type, value);
           },
           py::arg("name"), py::arg("type"), py::arg("value"))
      .def("append_attr",
           [](libdap::AttrTable& self, const string& name, const string& type,
              std::vector<std::string>* values) -> unsigned int {
             return self.append_attr(name, type, values);
           },
           py::arg("name"), py::arg("type"), py::arg("values"))
      .def("append_container",
           [](libdap::AttrTable& self, const string& name)
               -> libdap::AttrTable* { return self.append_container(name); },
           py::arg("name"), py::return_value_policy::reference_internal)
      .def("simple_find",
           [](libdap::AttrTable& self, const string& target)
               -> boost::optional<libdap::AttrTable::entry*> {
             auto it = self.simple_find(target);
             if (it == self.attr_end()) {
               return {};
             }
             return *it;
           },
           py::arg("target"), py::return_value_policy::reference_internal)
      .def("find_container",
           [](libdap::AttrTable& self, const string& name)
               -> libdap::AttrTable* { return self.find_container(name); },
           py::arg("name"), py::return_value_policy::reference_internal)
      .def("simple_find_container",
           [](libdap::AttrTable& self,
              const string& name) -> libdap::AttrTable* {
             return self.simple_find_container(name);
           },
           py::arg("name"), py::return_value_policy::reference_internal)
      .def("recursive_find",
           [](libdap::AttrTable& self, const string& name)
               -> libdap::AttrTable* { return recursive_find(&self, name); },
           py::arg("name"), py::return_value_policy::reference_internal)
      .def("get_attr_table",
           [](libdap::AttrTable& self, const string& name)
               -> libdap::AttrTable* { return self.get_attr_table(name); },
           py::arg("name"), py::return_value_policy::reference_internal)
      .def("get_type",
           [](libdap::AttrTable& self, const string& name) -> std::string {
             return self.get_type(name);
           },
           py::arg("name"))
      .def("get_attr_type",
           [](libdap::AttrTable& self, const string& name) -> libdap::AttrType {
             return self.get_attr_type(name);
           },
           py::arg("name"))
      .def("get_attr_num",
           [](libdap::AttrTable& self, const string& name) -> unsigned int {
             return self.get_attr_num(name);
           },
           py::arg("name"))
      .def("get_attr",
           [](libdap::AttrTable& self, const string& name,
              unsigned int i) -> std::string { return self.get_attr(name, i); },
           py::arg("name"), py::arg("i") = 0)
      .def("del_attr",
           [](libdap::AttrTable& self, const string& name, int i) -> void {
             self.del_attr(name, i);
           },
           py::arg("name"), py::arg("i") = -1)
      .def("get_attr_vector",
           [](libdap::AttrTable& self,
              const string& name) -> std::vector<std::string>* {
             return self.get_attr_vector(name);
           },
           py::arg("name"), py::return_value_policy::reference_internal)
      .def("__iter__",
           [](libdap::AttrTable& self) {
             return py::make_iterator(
                 self.attr_begin(), self.attr_end(),
                 py::return_value_policy::reference_internal);
           },
           py::keep_alive<0, 1>())
      .def("add_container_alias",
           [](libdap::AttrTable& self, const std::string& name,
              libdap::AttrTable* src) -> void {
             self.add_container_alias(name, src);
           },
           py::arg("name"), py::arg("src"))
      .def("add_value_alias",
           [](libdap::AttrTable& self, libdap::AttrTable* at,
              const std::string& name, const std::string& source) -> void {
             self.add_value_alias(at, name, source);
           },
           py::arg("at"), py::arg("name"), py::arg("src"))
      .def("attr_alias",
           [](libdap::AttrTable& self, const std::string& alias,
              libdap::AttrTable* at, const std::string& name) -> bool {
             return self.attr_alias(alias, at, name);
           },
           py::arg("alias"), py::arg("at"), py::arg("name"))
      .def("attr_alias",
           [](libdap::AttrTable& self, const std::string& alias,
              const std::string& name) -> bool {
             return self.attr_alias(alias, name);
           },
           py::arg("alias"), py::arg("name"))
      .def("print",
           [](libdap::AttrTable& self, std::string pad,
              bool dereference) -> std::string {
             std::ostringstream ss;
             self.print(ss, pad, dereference);
             return ss.str();
           },
           py::arg("pad") = "    ", py::arg("dereference") = false)
      .def("print_xml",
           [](libdap::AttrTable& self, std::string pad,
              bool dereference) -> std::string {
             std::ostringstream ss;
             self.print_xml(ss, pad, dereference);
             return ss.str();
           },
           py::arg("pad") = "    ", py::arg("dereference") = false)
      .def("print_xml_writer", &libdap::AttrTable::print_xml_writer)
      .def("print_dap4", &libdap::AttrTable::print_dap4)
      .def("dump", [](const libdap::AttrTable& self) -> std::string {
        std::ostringstream ss;
        self.dump(ss);
        return ss.str();
      });
}
