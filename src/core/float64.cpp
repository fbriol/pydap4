#include <cstdint>
#include <libdap/crc.h>
#include <libdap/Float64.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sstream>

namespace py = pybind11;

struct Float64 : public libdap::Float64 {
  using libdap::Float64::Float64;

  libdap::BaseType* ptr_duplicate() override {
    PYBIND11_OVERLOAD(libdap::BaseType*, /* Return type */
                      libdap::Float64,   /* Parent class */
                      ptr_duplicate,     /* Name of function in C++  */
    );
  }

  void compute_checksum(Crc32& checksum) override {
    PYBIND11_OVERLOAD(void,             /* Return type */
                      libdap::Float64,  /* Parent class */
                      compute_checksum, /* Name of function in C++  */
                      checksum          /* Arguments */
    );
  }

  void print_val(std::ostream& out, std::string space = "",
                 bool print_decl_p = true) override {
    PYBIND11_OVERLOAD(void,            /* Return type */
                      libdap::Float64, /* Parent class */
                      print_val,       /* Name of function in C++  */
                      out,             /* Arguments */
                      space, print_decl_p);
  }
};

void init_float64(py::module& m) {
  py::class_<libdap::Float64, Float64, libdap::BaseType>(m, "Float64")
      .def(py::init<const std::string&>(), py::arg("n"))
      .def(py::init<const std::string&, const std::string&>(), py::arg("n"),
           py::arg("d"))
      .def("width", &libdap::Float64::width, py::arg("constrained") = false)
      .def("ptr_duplicate", &libdap::Float64::ptr_duplicate)
      // .def("serialize", &libdap::Float64::serialize
      // .def("deserialize", &libdap::Float64::deserialize
      .def("compute_checksum", &libdap::Float64::compute_checksum)
      // .def("serialize", &libdap::Float64::serialize
      // .def("deserialize", &libdap::Float64::deserialize
      .def("set_value", &libdap::Float64::set_value, py::arg("value"))
      .def("value", &libdap::Float64::value)
      .def("print_val",
           [](libdap::Float64& self, std::string& space,
              bool print_decl_p) -> std::string {
             std::ostringstream ss;
             self.print_val(ss, space, print_decl_p);
             return ss.str();
           },
           py::arg("space") = "", py::arg("print_decl_p") = true)
      .def("dump", [](const libdap::Float64& self) -> std::string {
        std::ostringstream ss;
        self.dump(ss);
        return ss.str();
      });
}