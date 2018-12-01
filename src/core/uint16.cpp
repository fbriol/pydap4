#include <cstdint>
#include <libdap/crc.h>
#include <libdap/UInt16.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sstream>

namespace py = pybind11;

struct UInt16 : public libdap::UInt16 {
  using libdap::UInt16::UInt16;

  libdap::BaseType* ptr_duplicate() override {
    PYBIND11_OVERLOAD(libdap::BaseType*, /* Return type */
                      libdap::UInt16,     /* Parent class */
                      ptr_duplicate,     /* Name of function in C++  */
    );
  }

  void compute_checksum(Crc32& checksum) override {
    PYBIND11_OVERLOAD(void,             /* Return type */
                      libdap::UInt16,    /* Parent class */
                      compute_checksum, /* Name of function in C++  */
                      checksum          /* Arguments */
    );
  }

  void print_val(std::ostream& out, std::string space = "",
                 bool print_decl_p = true) override {
    PYBIND11_OVERLOAD(void,          /* Return type */
                      libdap::UInt16, /* Parent class */
                      print_val,     /* Name of function in C++  */
                      out,           /* Arguments */
                      space, print_decl_p);
  }
};

void init_uint16(py::module& m) {
  py::class_<libdap::UInt16, UInt16, libdap::BaseType>(m, "UInt16")
      .def(py::init<const std::string&>(), py::arg("n"))
      .def(py::init<const std::string&, const std::string&>(), py::arg("n"),
           py::arg("d"))
      .def("width", &libdap::UInt16::width, py::arg("constrained") = false)
      .def("ptr_duplicate", &libdap::UInt16::ptr_duplicate)
      // .def("serialize", &libdap::UInt16::serialize
      // .def("deserialize", &libdap::UInt16::deserialize
      .def("compute_checksum", &libdap::UInt16::compute_checksum)
      // .def("serialize", &libdap::UInt16::serialize
      // .def("deserialize", &libdap::UInt16::deserialize
      .def("set_value", &libdap::UInt16::set_value, py::arg("value"))
      .def("value", &libdap::UInt16::value)
      .def("print_val",
           [](libdap::UInt16& self, std::string& space,
              bool print_decl_p) -> std::string {
             std::ostringstream ss;
             self.print_val(ss, space, print_decl_p);
             return ss.str();
           },
           py::arg("space") = "", py::arg("print_decl_p") = true)
      .def("dump", [](const libdap::UInt16& self) -> std::string {
        std::ostringstream ss;
        self.dump(ss);
        return ss.str();
      });
}