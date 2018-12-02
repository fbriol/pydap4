#include <Byte.h>
#include <crc.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cstdint>
#include <sstream>

namespace py = pybind11;

struct Byte : public libdap::Byte {
  using libdap::Byte::Byte;

  libdap::BaseType* ptr_duplicate() override {
    PYBIND11_OVERLOAD(libdap::BaseType*, /* Return type */
                      libdap::Byte,      /* Parent class */
                      ptr_duplicate,     /* Name of function in C++  */
    );
  }

  void compute_checksum(Crc32& checksum) override {
    PYBIND11_OVERLOAD(void,             /* Return type */
                      libdap::Byte,     /* Parent class */
                      compute_checksum, /* Name of function in C++  */
                      checksum          /* Arguments */
    );
  }

  void print_val(std::ostream& out, std::string space = "",
                 bool print_decl_p = true) override {
    PYBIND11_OVERLOAD(void,         /* Return type */
                      libdap::Byte, /* Parent class */
                      print_val,    /* Name of function in C++  */
                      out,          /* Arguments */
                      space, print_decl_p);
  }
};

void init_byte(py::module& m) {
  py::class_<libdap::Byte, Byte, libdap::BaseType>(m, "Byte")
      .def(py::init<const std::string&>(), py::arg("n"))
      .def(py::init<const std::string&, const std::string&>(), py::arg("n"),
           py::arg("d"))
      .def("width", &libdap::Byte::width, py::arg("constrained") = false)
      .def("ptr_duplicate", &libdap::Byte::ptr_duplicate)
      // .def("serialize", &libdap::Byte::serialize
      // .def("deserialize", &libdap::Byte::deserialize
      .def("compute_checksum", &libdap::Byte::compute_checksum)
      // .def("serialize", &libdap::Byte::serialize
      // .def("deserialize", &libdap::Byte::deserialize
      .def("set_value", &libdap::Byte::set_value, py::arg("value"))
      .def("value", &libdap::Byte::value)
      .def("print_val",
           [](libdap::Byte& self, std::string& space,
              bool print_decl_p) -> std::string {
             std::ostringstream ss;
             self.print_val(ss, space, print_decl_p);
             return ss.str();
           },
           py::arg("space") = "", py::arg("print_decl_p") = true)
      .def("dump", [](const libdap::Byte& self) -> std::string {
        std::ostringstream ss;
        self.dump(ss);
        return ss.str();
      });
}