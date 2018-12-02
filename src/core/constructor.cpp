#include <Constructor.h>
#include <crc.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cstdint>
#include <sstream>

namespace py = pybind11;

struct Constructor : public libdap::Constructor {
  using libdap::Constructor::Constructor;

  libdap::BaseType* ptr_duplicate() override {
    PYBIND11_OVERLOAD(libdap::BaseType*,   /* Return type */
                      libdap::Constructor, /* Parent class */
                      ptr_duplicate,       /* Name of function in C++  */
    );
  }

  void compute_checksum(Crc32& checksum) override {
    PYBIND11_OVERLOAD(void,                /* Return type */
                      libdap::Constructor, /* Parent class */
                      compute_checksum,    /* Name of function in C++  */
                      checksum             /* Arguments */
    );
  }

  void print_val(std::ostream& out, std::string space = "",
                 bool print_decl_p = true) override {
    PYBIND11_OVERLOAD(void,                /* Return type */
                      libdap::Constructor, /* Parent class */
                      print_val,           /* Name of function in C++  */
                      out,                 /* Arguments */
                      space, print_decl_p);
  }
};

static inline libdap::Constructor::Vars_iter get_iter(
    libdap::Constructor& self, const libdap::BaseType* var) {
  for (auto it = self.var_begin(); it != self.var_end(); ++it) {
    if ((*it)->name() == var->name()) {
      return it;
    }
  }
  throw std::invalid_argument(
      "The requested variable does not belong to this instance");
}

void init_constructor(py::module& m) {
  py::class_<libdap::Constructor, Constructor, libdap::BaseType>(m,
                                                                 "Constructor")
      // .def("transform_to_dap4", &libdap::Constructor::transform_to_dap4
      .def("FQN", &libdap::Constructor::FQN)
      .def("element_count", &libdap::Constructor::element_count,
           py::arg("leaves") = false)
      .def("set_send_p", &libdap::Constructor::set_send_p, py::arg("state"))
      .def("set_read_p", &libdap::Constructor::set_read_p, py::arg("state"))
      .def("width", &libdap::Constructor::width, py::arg("constrained") = false)
      .def("var",
           [](libdap::Constructor& self, const std::string& name,
              bool exact_match,
              libdap::BaseType::btp_stack* s) -> libdap::BaseType* {
             return self.var(name, exact_match, s);
           },
           py::arg("name") = "", py::arg("exact_match") = true,
           py::arg("s") = nullptr, py::return_value_policy::reference_internal)
      .def("var",
           [](libdap::Constructor& self, const std::string& name,
              libdap::BaseType::btp_stack& s) -> libdap::BaseType* {
             return self.var(name, s);
           },
           py::arg("name"), py::arg("s"),
           py::return_value_policy::reference_internal)
      .def("__iter__",
           [](libdap::Constructor& self) {
             return py::make_iterator(
                 self.var_begin(), self.var_end(),
                 py::return_value_policy::reference_internal);
           },
           py::keep_alive<0, 1>())
      // .def("add_var", &libdap::Constructor::add_var
      // .def("add_var_nocopy", &libdap::Constructor::add_var_nocopy
      .def("del_var",
           [](libdap::Constructor& self, const std::string& name) -> void {
             self.del_var(name);
           },
           py::arg("name"))
      .def("del_var",
           [](libdap::Constructor& self, const libdap::BaseType* var) -> void {
             self.del_var(get_iter(self, var));
           },
           py::arg("var"))
      .def("read", &libdap::Constructor::read)
      //  .def("intern_data", &libdap::Constructor::intern_data
      //  .def("serialize", &libdap::Constructor::serialize
      //  .def("deserialize", &libdap::Constructor::deserialize
      .def("compute_checksum", &libdap::Constructor::compute_checksum,
           py::arg("checksum"))
      .def("intern_data",
           [](libdap::Constructor& self) -> void { self.intern_data(); })
      //  .def("serialize", &libdap::Constructor::serialize
      //  .def("deserialize", &libdap::Constructor::deserialize
      .def("is_linear", &libdap::Constructor::is_linear)
      .def("set_in_selection", &libdap::Constructor::set_in_selection,
           py::arg("state"))
      .def("print_decl",
           [](libdap::Constructor& self, std::string space, bool print_semi,
              bool constraint_info, bool constrained) -> std::string {
             std::ostringstream ss;
             self.print_decl(ss, space, print_semi, constraint_info,
                             constrained);
             return ss.str();
           },
           py::arg("space") = "    ", py::arg("print_semi") = true,
           py::arg("constraint_info") = false, py::arg("constrained") = false)
      .def("print_xml",
           [](libdap::Constructor& self, std::string space,
              bool constrained) -> std::string {
             std::ostringstream ss;
             self.print_xml(ss, space, constrained);
             return ss.str();
           },
           py::arg("space") = "    ", py::arg("constrained") = false)
      .def("print_dap4", &libdap::Constructor::print_dap4, py::arg("xml"),
           py::arg("constrained") = false)
      .def("print_xml_writer", &libdap::Constructor::print_xml_writer,
           py::arg("xml"), py::arg("constrained") = false)
      .def("check_semantics", &libdap::Constructor::check_semantics,
           py::arg("msg"), py::arg("all") = false)
      .def("transfer_attributes", &libdap::Constructor::transfer_attributes,
           py::arg("at"))
      // .def("make_dropped_vars_attr_table",
      // &libdap::Constructor::make_dropped_vars_attr_table
      .def("dump", [](libdap::Constructor& self) -> std::string {
        std::ostringstream ss;
        self.dump(ss);
        return ss.str();
      });
}