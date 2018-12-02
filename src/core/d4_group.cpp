#include <Array.h>
#include <D4Group.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

struct D4Group : public libdap::D4Group {
  using libdap::D4Group::D4Group;

  void print_val(std::ostream& out, std::string space = "",
                 bool print_decl_p = true) override {
    PYBIND11_OVERLOAD(void,            /* Return type */
                      libdap::D4Group, /* Parent class */
                      print_val,       /* Name of function in C++  */
                      out,             /* Arguments */
                      space, print_decl_p);
  }
};

static inline libdap::D4Group::groupsIter get_iter(libdap::D4Group& self,
                                                   const libdap::D4Group* g) {
  for (auto it = self.grp_begin(); it != self.grp_end(); ++it) {
    if ((*it)->name() == g->name()) {
      return it;
    }
  }
  throw std::invalid_argument(
      "The requested group does not belong to this instance");
}

void init_d4_group(py::module& m) {
  py::class_<libdap::D4Group, D4Group, libdap::Constructor>(m, "D4Group")
      .def(py::init<const std::string&>(), py::arg("name"))
      .def(py::init<const std::string&, const std::string&>(), py::arg("name"),
           py::arg("dataset"))
      .def("dims", &libdap::D4Group::dims,
           py::return_value_policy::reference_internal)
      .def("FQN", &libdap::D4Group::FQN)
      .def("find_dim", &libdap::D4Group::find_dim, py::arg("path"),
           py::return_value_policy::reference_internal)
      .def("find_map_source", &libdap::D4Group::find_map_source,
           py::arg("path"), py::return_value_policy::reference_internal)
      .def("find_enum_def", &libdap::D4Group::find_enum_def, py::arg("path"),
           py::return_value_policy::reference_internal)
      .def("enum_defs", &libdap::D4Group::enum_defs,
           py::return_value_policy::reference_internal)
      .def("find_first_var_that_uses_dimension",
           &libdap::D4Group::find_first_var_that_uses_dimension, py::arg("dim"),
           py::return_value_policy::reference_internal)
      .def("find_first_var_that_uses_enumeration",
           &libdap::D4Group::find_first_var_that_uses_enumeration,
           py::arg("enum_def"), py::return_value_policy::reference_internal)
      .def("find_var", &libdap::D4Group::find_var, py::arg("name"),
           py::return_value_policy::reference_internal)
      .def("__iter__",
           [](libdap::D4Group& self) {
             return py::make_iterator(
                 self.grp_begin(), self.grp_end(),
                 py::return_value_policy::reference_internal);
           },
           py::keep_alive<0, 1>())
      .def("add_group", &libdap::D4Group::add_group, py::arg("group"))
      .def("add_group_nocopy", &libdap::D4Group::add_group_nocopy,
           py::arg("group"), py::keep_alive<1, 2>())
      .def("insert_group_nocopy",
           [](libdap::D4Group& self, libdap::D4Group* g,
              libdap::D4Group* before) -> void {
             self.insert_group_nocopy(g, get_iter(self, before));
           },
           py::arg("group"), py::arg("before"), py::keep_alive<1, 2>())
      .def("find_child_grp", &libdap::D4Group::find_child_grp, py::arg("name"),
           py::return_value_policy::reference_internal)
      .def("request_size", &libdap::D4Group::request_size,
           py::arg("constrained"))
      .def("set_send_p", &libdap::D4Group::set_send_p, py::arg("state"))
      .def("set_read_p", &libdap::D4Group::set_read_p, py::arg("state"))
      .def("intern_data", &libdap::D4Group::intern_data)
      //  .def("serialize", &libdap::D4Group::serialize
      //  .def("deserialize", &libdap::D4Group::deserialize
      .def("print_dap4", &libdap::D4Group::print_dap4, py::arg("xml"),
           py::arg("constrained") = false)
      //  .def("transform_to_dap2", &libdap::D4Group::transform_to_dap2
      //  .def("transform_to_dap2", &libdap::D4Group::transform_to_dap2
      ;
}