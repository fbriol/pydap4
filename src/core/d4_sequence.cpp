#include <D4Sequence.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

struct D4Sequence : public libdap::D4Sequence {
  using libdap::D4Sequence::D4Sequence;

  libdap::BaseType* ptr_duplicate() override {
    PYBIND11_OVERLOAD(libdap::BaseType*,  /* Return type */
                      libdap::D4Sequence, /* Parent class */
                      ptr_duplicate,      /* Name of function in C++  */
    );
  }

  void print_val(std::ostream& out, std::string space = "",
                 bool print_decl_p = true) override {
    PYBIND11_OVERLOAD(void,               /* Return type */
                      libdap::D4Sequence, /* Parent class */
                      print_val,          /* Name of function in C++  */
                      out,                /* Arguments */
                      space, print_decl_p);
  }
};

void init_d4_sequence(py::module& m) {
  py::class_<libdap::D4Sequence, D4Sequence, libdap::Constructor>(m,
                                                                  "D4Sequence")
      .def(py::init<const std::string&>(), py::arg("name"))
      .def(py::init<const std::string&, const std::string&>(), py::arg("name"),
           py::arg("dataset"))
      .def("ptr_duplicate", &libdap::D4Sequence::ptr_duplicate,
           py::return_value_policy::reference_internal)
      .def("clear_local_data", &libdap::D4Sequence::clear_local_data)
      .def("length", &libdap::D4Sequence::length)
      .def("set_length", &libdap::D4Sequence::set_length, py::arg("count"))
      .def("read_next_instance", &libdap::D4Sequence::read_next_instance,
           py::arg("filter"))
      // intern_data
      // serialize
      // deserialize
      // intern_data
      // serialize
      // deserialize
      // TODO .../...
      ;
}