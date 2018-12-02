#include <D4EnumDefs.h>
#include <D4Group.h>
#include <Type.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

static inline libdap::D4EnumDefs::D4EnumDefIter get_iter(
    libdap::D4EnumDefs& self, const libdap::D4EnumDef* enum_def) {
  for (auto it = self.enum_begin(); it != self.enum_end(); ++it) {
    if ((*it)->name() == enum_def->name()) {
      return it;
    }
  }
  throw std::invalid_argument(
      "The requested enumeration does not belong to this instance");
}

void init_d4_enum_defs(py::module& m) {
  py::class_<libdap::D4EnumDef>(m, "D4EnumDef")
      .def(py::init<>())
      .def(py::init<const std::string&, libdap::Type, libdap::D4EnumDefs*>(),
           py::arg("name"), py::arg("type"), py::arg("e") = nullptr,
           py::keep_alive<1, 4>())
      .def("name", &libdap::D4EnumDef::name)
      .def("set_name", &libdap::D4EnumDef::set_name, py::arg("name"))
      .def("type", &libdap::D4EnumDef::type)
      .def("set_type", &libdap::D4EnumDef::set_type, py::arg("type"))
      .def("parent", &libdap::D4EnumDef::parent,
           py::return_value_policy::reference_internal)
      .def("set_parent",
           [](libdap::D4EnumDef& self, libdap::D4EnumDefs* d) -> void {
             self.set_parent(d);
           },
           py::arg("d"), py::keep_alive<1, 2>())
      .def("empty", &libdap::D4EnumDef::empty)
      .def("add_value", &libdap::D4EnumDef::add_value, py::arg("label"),
           py::arg("value"))
      .def("__iter__",
           [](libdap::D4EnumDef& self) {
             return py::make_iterator(self.value_begin(), self.value_end());
           },
           py::keep_alive<0, 1>())
      .def("is_valid_enum_value", &libdap::D4EnumDef::is_valid_enum_value)
      .def("print_dap4", &libdap::D4EnumDef::print_dap4, py::arg("xml"));

  py::class_<libdap::D4EnumDefs>(m, "D4EnumDefs")
      .def(py::init<>())
      .def("empty", &libdap::D4EnumDefs::empty)
      .def("parent", &libdap::D4EnumDefs::parent,
           py::return_value_policy::reference_internal)
      .def("set_parent",
           [](libdap::D4EnumDefs& self, libdap::D4Group* g) -> void {
             self.set_parent(g);
           },
           py::arg("g"), py::keep_alive<1, 2>())
      .def("add_enum", &libdap::D4EnumDefs::add_enum, py::arg("enum"))
      .def("add_enum_nocopy", &libdap::D4EnumDefs::add_enum_nocopy,
           py::arg("enum"), py::keep_alive<1, 2>())
      .def("__iter__",
           [](libdap::D4EnumDefs& self) {
             return py::make_iterator(
                 self.enum_begin(), self.enum_end(),
                 py::return_value_policy::reference_internal);
           },
           py::keep_alive<0, 1>())
      .def("find_enum_def", &libdap::D4EnumDefs::find_enum_def, py::arg("name"))
      .def("insert_dim",
           [](libdap::D4EnumDefs& self, libdap::D4EnumDef* enum_def,
              libdap::D4EnumDef* before) -> void {
             self.insert_enum(enum_def, get_iter(self, before));
           },
           py::arg("enum_def"), py::arg("before"))
      .def("print_dap4", &libdap::D4EnumDefs::print_dap4, py::arg("xml"),
           py::arg("constrained") = false);
}