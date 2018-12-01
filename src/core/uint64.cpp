#include <cstdint>
#include <libdap/crc.h>
#include <libdap/UInt64.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sstream>

namespace py = pybind11;

struct UInt64 : public libdap::UInt64 {
  using libdap::UInt64::UInt64;

  libdap::BaseType* ptr_duplicate() override {
    PYBIND11_OVERLOAD(libdap::BaseType*, /* Return type */
                      libdap::UInt64,    /* Parent class */
                      ptr_duplicate,     /* Name of function in C++  */
    );
  }

  void compute_checksum(Crc32& checksum) override {
    PYBIND11_OVERLOAD(void,             /* Return type */
                      libdap::UInt64,   /* Parent class */
                      compute_checksum, /* Name of function in C++  */
                      checksum          /* Arguments */
    );
  }

  void print_val(std::ostream& out, std::string space = "",
                 bool print_decl_p = true) override {
    PYBIND11_OVERLOAD(void,           /* Return type */
                      libdap::UInt64, /* Parent class */
                      print_val,      /* Name of function in C++  */
                      out,            /* Arguments */
                      space, print_decl_p);
  }
};

void init_uint64(py::module& m) {
  py::class_<libdap::UInt64, UInt64, libdap::BaseType>(m, "UInt64")
      .def(py::init<const std::string&>(), py::arg("n"))
      .def(py::init<const std::string&, const std::string&>(), py::arg("n"),
           py::arg("d"))
      .def("width", &libdap::UInt64::width, py::arg("constrained") = false)
      .def("ptr_duplicate", &libdap::UInt64::ptr_duplicate)
      // .def("serialize", &libdap::UInt64::serialize
      // .def("deserialize", &libdap::UInt64::deserialize
      .def("compute_checksum", &libdap::UInt64::compute_checksum)
      // .def("serialize", &libdap::UInt64::serialize
      // .def("deserialize", &libdap::UInt64::deserialize
      .def("set_value", &libdap::UInt64::set_value, py::arg("value"))
      .def("value", &libdap::UInt64::value)
      .def("print_val",
           [](libdap::UInt64& self, std::string& space,
              bool print_decl_p) -> std::string {
             std::ostringstream ss;
             self.print_val(ss, space, print_decl_p);
             return ss.str();
           },
           py::arg("space") = "", py::arg("print_decl_p") = true)
      .def("dump", [](const libdap::UInt64& self) -> std::string {
        std::ostringstream ss;
        self.dump(ss);
        return ss.str();
      });
}