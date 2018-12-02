#include <UInt32.h>
#include <crc.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cstdint>
#include <sstream>

namespace py = pybind11;

struct UInt32 : public libdap::UInt32 {
  using libdap::UInt32::UInt32;

  libdap::BaseType* ptr_duplicate() override {
    PYBIND11_OVERLOAD(libdap::BaseType*, /* Return type */
                      libdap::UInt32,    /* Parent class */
                      ptr_duplicate,     /* Name of function in C++  */
    );
  }

  void compute_checksum(Crc32& checksum) override {
    PYBIND11_OVERLOAD(void,             /* Return type */
                      libdap::UInt32,   /* Parent class */
                      compute_checksum, /* Name of function in C++  */
                      checksum          /* Arguments */
    );
  }

  void print_val(std::ostream& out, std::string space = "",
                 bool print_decl_p = true) override {
    PYBIND11_OVERLOAD(void,           /* Return type */
                      libdap::UInt32, /* Parent class */
                      print_val,      /* Name of function in C++  */
                      out,            /* Arguments */
                      space, print_decl_p);
  }
};

void init_uint32(py::module& m) {
  py::class_<libdap::UInt32, UInt32, libdap::BaseType>(m, "UInt32")
      .def(py::init<const std::string&>(), py::arg("n"))
      .def(py::init<const std::string&, const std::string&>(), py::arg("n"),
           py::arg("d"))
      .def("width", &libdap::UInt32::width, py::arg("constrained") = false)
      .def("ptr_duplicate", &libdap::UInt32::ptr_duplicate)
      // .def("serialize", &libdap::UInt32::serialize
      // .def("deserialize", &libdap::UInt32::deserialize
      .def("compute_checksum", &libdap::UInt32::compute_checksum)
      // .def("serialize", &libdap::UInt32::serialize
      // .def("deserialize", &libdap::UInt32::deserialize
      .def("set_value", &libdap::UInt32::set_value, py::arg("value"))
      .def("value", &libdap::UInt32::value)
      .def("print_val",
           [](libdap::UInt32& self, std::string& space,
              bool print_decl_p) -> std::string {
             std::ostringstream ss;
             self.print_val(ss, space, print_decl_p);
             return ss.str();
           },
           py::arg("space") = "", py::arg("print_decl_p") = true)
      .def("dump", [](const libdap::UInt32& self) -> std::string {
        std::ostringstream ss;
        self.dump(ss);
        return ss.str();
      });
}