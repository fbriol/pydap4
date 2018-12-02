#include <Sequence.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sstream>

namespace py = pybind11;

struct Sequence : public libdap::Sequence {
  using libdap::Sequence::Sequence;

  libdap::BaseType* ptr_duplicate() override {
    PYBIND11_OVERLOAD(libdap::BaseType*, /* Return type */
                      libdap::Sequence,  /* Parent class */
                      ptr_duplicate,     /* Name of function in C++  */
    );
  }

  void print_val(std::ostream& out, std::string space = "",
                 bool print_decl_p = true) override {
    PYBIND11_OVERLOAD(void,             /* Return type */
                      libdap::Sequence, /* Parent class */
                      print_val,        /* Name of function in C++  */
                      out,              /* Arguments */
                      space, print_decl_p);
  }
};

void init_sequence(py::module& m) {
  py::class_<libdap::Sequence, Sequence, libdap::Constructor>(m, "Sequence")
      .def(py::init<const std::string&>(), py::arg("n"))
      .def(py::init<const std::string&, const string&>(), py::arg("n"),
           py::arg("d"))
      .def("ptr_duplicate", &libdap::Sequence::ptr_duplicate)
      .def("clear_local_data", &libdap::Sequence::clear_local_data)
      //   .def("transform_to_dap4", &libdap::Sequence::transform_to_dap4
      .def("is_dap2_only_type", &libdap::Sequence::is_dap2_only_type)
      .def("__str__", &libdap::Sequence::toString)
      .def("is_linear", &libdap::Sequence::is_linear)
      .def("length", &libdap::Sequence::length)
      .def("number_of_rows", &libdap::Sequence::number_of_rows)
      //   .def("read_row", &libdap::Sequence::read_row
      //   .def("intern_data", &libdap::Sequence::intern_data
      //   .def("serialize", &libdap::Sequence::serialize
      //   .def("deserialize", &libdap::Sequence::deserialize
      .def("reset_row_number",
           [](libdap::Sequence& self) -> void { self.reset_row_number(); })
      .def("reset_row_number",
           [](libdap::Sequence& self, bool recur) -> void {
             self.reset_row_number(recur);
           },
           py::arg("recur"))
      .def("increment_row_number", &libdap::Sequence::increment_row_number,
           py::arg("i"))
      .def("get_row_number", &libdap::Sequence::get_row_number)
      .def("get_starting_row_number",
           &libdap::Sequence::get_starting_row_number)
      .def("get_row_stride", &libdap::Sequence::get_row_stride)
      .def("get_ending_row_number", &libdap::Sequence::get_ending_row_number)
      .def("set_row_number_constraint",
           &libdap::Sequence::set_row_number_constraint, py::arg("start"),
           py::arg("stop"), py::arg("stride") = 1)
      .def("get_unsent_data", &libdap::Sequence::get_unsent_data)
      .def("set_unsent_data", &libdap::Sequence::set_unsent_data,
           py::arg("usd"))
      //   .def("set_value", &libdap::Sequence::set_value, py::arg("values"))
      //   .def("value", &libdap::Sequence::value)
      .def("var_value",
           [](libdap::Sequence& self, size_t row, const std::string& name)
               -> libdap::BaseType* { return self.var_value(row, name); },
           py::arg("row"), py::arg("name"),
           py::return_value_policy::reference_internal)
      .def("var_value",
           [](libdap::Sequence& self, size_t row,
              size_t i) -> libdap::BaseType* { return self.var_value(row, i); },
           py::arg("row"), py::arg("i"),
           py::return_value_policy::reference_internal)
      //   .def("row_value", &libdap::Sequence::row_value
      .def("print_one_row",
           [](libdap::Sequence& self, int row, std::string space,
              bool print_row_num) -> std::string {
             std::ostringstream ss;
             self.print_one_row(ss, row, space, print_row_num);
             return ss.str();
           },
           py::arg("row"), py::arg("space"), py::arg("print_row_num") = false)
      .def("print_val_by_rows",
           [](libdap::Sequence& self, std::string space, bool print_decl_p,
              bool print_row_numbers) -> std::string {
             std::ostringstream ss;
             self.print_val_by_rows(ss, space, print_decl_p, print_row_numbers);
             return ss.str();
           },
           py::arg("space") = "", py::arg("print_decl_p") = true,
           py::arg("print_row_numbers") = true)
      .def("print_val",
           [](libdap::Sequence& self, std::string space,
              bool print_decl_p) -> std::string {
             std::ostringstream ss;
             self.print_val(ss, space, print_decl_p);
             return ss.str();
           },
           py::arg("space") = "", py::arg("print_decl_p") = true)
      .def("set_leaf_p", &libdap::Sequence::set_leaf_p, py::arg("state"))
      .def("is_leaf_sequence", &libdap::Sequence::is_leaf_sequence)
      .def("set_leaf_sequence", &libdap::Sequence::set_leaf_sequence,
           py::arg("lvl") = 1)
      .def("dump", [](const libdap::Sequence& self) -> std::string {
        std::ostringstream ss;
        self.dump(ss);
        return ss.str();
      });
}