#include <libdap/Grid.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sstream>

namespace py = pybind11;
struct Grid : public libdap::Grid {
  using libdap::Grid::Grid;

  libdap::BaseType* ptr_duplicate() override {
    PYBIND11_OVERLOAD(libdap::BaseType*, /* Return type */
                      libdap::Grid,      /* Parent class */
                      ptr_duplicate,     /* Name of function in C++  */
    );
  }

  void print_val(std::ostream& out, std::string space = "",
                 bool print_decl_p = true) override {
    PYBIND11_OVERLOAD(void,         /* Return type */
                      libdap::Grid, /* Parent class */
                      print_val,    /* Name of function in C++  */
                      out,          /* Arguments */
                      space, print_decl_p);
  }
};

void init_grid(py::module& m) {
  py::class_<libdap::Grid, Grid, libdap::Constructor>(m, "Grid")
      .def(py::init<const std::string&>(), py::arg("n"))
      .def(py::init<const std::string&, const std::string&>(), py::arg("n"),
           py::arg("d"))
      .def("ptr_duplicate", &libdap::Grid::ptr_duplicate)
      //   .def("transform_to_dap4", &libdap::Grid::transform_to_dap4,
      //        py::arg("root"), py::arg("container"))
      .def("is_dap2_only_type", &libdap::Grid::is_dap2_only_type)
      .def("add_var", &libdap::Grid::add_var, py::arg("bt"),
           py::arg("part") = libdap::Part::nil)
      .def("add_var_nocopy", &libdap::Grid::add_var_nocopy, py::arg("bt"),
           py::arg("part") = libdap::Part::nil, py::keep_alive<1, 2>())
      .def("set_array", &libdap::Grid::set_array, py::arg("array"),
           py::keep_alive<1, 2>())
      .def("add_map",
           [](libdap::Grid& self, libdap::Array* map) -> libdap::Array* {
             return self.add_map(map, false);
           },
           py::arg("map"), py::keep_alive<1, 2>(),
           py::return_value_policy::reference_internal)
      .def("prepend_map",
           [](libdap::Grid& self, libdap::Array* map) -> libdap::Array* {
             return self.prepend_map(map, false);
           },
           py::arg("map"), py::keep_alive<1, 2>(),
           py::return_value_policy::reference_internal)
      .def("get_array", &libdap::Grid::get_array,
           py::return_value_policy::reference_internal)
      .def("components", &libdap::Grid::components,
           py::arg("constrained") = false)
      .def("projection_yields_grid", &libdap::Grid::projection_yields_grid)
      .def("clear_constraint", &libdap::Grid::clear_constraint)
      .def("print_decl",
           [](libdap::Grid& self, std::string space, bool print_semi,
              bool constraint_info, bool constrained) -> std::string {
             std::ostringstream ss;
             self.print_decl(ss, space, print_semi, constraint_info,
                             constrained);
             return ss.str();
           },
           py::arg("space") = "    ", py::arg("print_semi") = true,
           py::arg("constraint_info") = false, py::arg("constrained") = false)
      .def("print_xml",
           [](libdap::Grid& self, std::string space,
              bool constrained) -> std::string {
             std::ostringstream ss;
             self.print_xml(ss, space, constrained);
             return ss.str();
           },
           py::arg("space") = "    ", py::arg("constrained") = false)
      .def("print_xml_writer", &libdap::Grid::print_xml_writer, py::arg("xml"),
           py::arg("constrained") = false)
      .def("print_val",
           [](libdap::Grid& self, std::string space,
              bool print_decl_p) -> std::string {
             std::ostringstream ss;
             self.print_val(ss, space, print_decl_p);
             return ss.str();
           },
           py::arg("space") = "", py::arg("print_decl_p") = true)
      .def("transfer_attributes", &libdap::Grid::transfer_attributes,
           py::arg("at"))
      .def("check_semantics", &libdap::Grid::check_semantics, py::arg("msg"),
           py::arg("all") = false)
      .def("dump",
           [](libdap::Grid& self) -> std::string {
             std::ostringstream ss;
             self.dump(ss);
             return ss.str();
           })
      .def("__iter__",
           [](libdap::Grid& self) {
             return py::make_iterator(
                 self.map_begin(), self.map_end(),
                 py::return_value_policy::reference_internal);
           },
           py::keep_alive<0, 1>());
}
