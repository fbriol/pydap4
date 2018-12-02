#include <Float32.h>
#include <crc.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cstdint>
#include <sstream>

namespace py = pybind11;

struct Float32 : public libdap::Float32 {
  using libdap::Float32::Float32;

  libdap::BaseType* ptr_duplicate() override {
    PYBIND11_OVERLOAD(libdap::BaseType*, /* Return type */
                      libdap::Float32,   /* Parent class */
                      ptr_duplicate,     /* Name of function in C++  */
    );
  }

  void compute_checksum(Crc32& checksum) override {
    PYBIND11_OVERLOAD(void,             /* Return type */
                      libdap::Float32,  /* Parent class */
                      compute_checksum, /* Name of function in C++  */
                      checksum          /* Arguments */
    );
  }

  void print_val(std::ostream& out, std::string space = "",
                 bool print_decl_p = true) override {
    PYBIND11_OVERLOAD(void,            /* Return type */
                      libdap::Float32, /* Parent class */
                      print_val,       /* Name of function in C++  */
                      out,             /* Arguments */
                      space, print_decl_p);
  }
};

void init_float32(py::module& m) {
  py::class_<libdap::Float32, Float32, libdap::BaseType>(m, "Float32")
      .def(py::init<const std::string&>(), py::arg("n"))
      .def(py::init<const std::string&, const std::string&>(), py::arg("n"),
           py::arg("d"))
      .def("width", &libdap::Float32::width, py::arg("constrained") = false)
      .def("ptr_duplicate", &libdap::Float32::ptr_duplicate)
      // .def("serialize", &libdap::Float32::serialize
      // .def("deserialize", &libdap::Float32::deserialize
      .def("compute_checksum", &libdap::Float32::compute_checksum)
      // .def("serialize", &libdap::Float32::serialize
      // .def("deserialize", &libdap::Float32::deserialize
      .def("set_value", &libdap::Float32::set_value, py::arg("value"))
      .def("value", &libdap::Float32::value)
      .def("print_val",
           [](libdap::Float32& self, std::string& space,
              bool print_decl_p) -> std::string {
             std::ostringstream ss;
             self.print_val(ss, space, print_decl_p);
             return ss.str();
           },
           py::arg("space") = "", py::arg("print_decl_p") = true)
      .def("dump", [](const libdap::Float32& self) -> std::string {
        std::ostringstream ss;
        self.dump(ss);
        return ss.str();
      });
}