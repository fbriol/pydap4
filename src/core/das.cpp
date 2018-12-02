#include <DAS.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sstream>

namespace py = pybind11;

void init_das(py::module& m) {
  py::class_<libdap::DAS>(m, "DAS")
      .def(py::init<>())
      .def("container_name",
           [](const libdap::DAS& self) -> std::string {
             return self.container_name();
           })
      .def("container_name",
           [](libdap::DAS& self, const std::string& cn) -> void {
             self.container_name(cn);
           })
      .def("container", &libdap::DAS::container,
           py::return_value_policy::reference_internal)
      .def("get_top_level_attributes", &libdap::DAS::get_top_level_attributes,
           py::return_value_policy::reference_internal)
      .def("erase", &libdap::DAS::erase)
      .def("get_size", &libdap::DAS::get_size)
      .def("get_name", &libdap::DAS::get_name)
      .def(
          "get_table",
          [](libdap::DAS& self, const std::string& name) -> libdap::AttrTable* {
            return self.get_table(name);
          },
          py::arg("table"), py::return_value_policy::reference_internal)
      .def("parse",
           [](libdap::DAS& self, const std::string& fname) -> void {
             self.parse(fname);
           },
           py::arg("fname"))
      .def(
          "get_table",
          [](libdap::DAS& self, const std::string& name) -> libdap::AttrTable* {
            return self.get_table(name);
          },
          py::arg("name"), py::return_value_policy::reference_internal)
      .def("add_table",
           [](libdap::DAS& self, const std::string& name,
              libdap::AttrTable* at) -> libdap::AttrTable* {
             return self.add_table(name, new libdap::AttrTable(*at));
           },
           py::arg("name"), py::arg("at"),
           py::return_value_policy::reference_internal)
      .def("print",
           [](libdap::DAS& self, bool dereference) -> std::string {
             std::ostringstream ss;
             self.print(ss);
             return ss.str();
           },
           py::arg("dereference") = false)
      .def("dump",
           [](const libdap::DAS& self) -> std::string {
             std::ostringstream ss;
             self.dump(ss);
             return ss.str();
           })
      .def("__iter__",
           [](libdap::DAS& self) {
             return py::make_iterator(
                 self.var_begin(), self.var_end(),
                 py::return_value_policy::reference_internal);
           },
           py::keep_alive<0, 1>());
}