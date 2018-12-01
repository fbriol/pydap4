#include <cstdint>
#include <libdap/crc.h>
#include <libdap/Int16.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sstream>

namespace py = pybind11;

struct Int16 : public libdap::Int16 {
  using libdap::Int16::Int16;

  libdap::BaseType* ptr_duplicate() override {
    PYBIND11_OVERLOAD(libdap::BaseType*, /* Return type */
                      libdap::Int16,     /* Parent class */
                      ptr_duplicate,     /* Name of function in C++  */
    );
  }

  void compute_checksum(Crc32& checksum) override {
    PYBIND11_OVERLOAD(void,             /* Return type */
                      libdap::Int16,    /* Parent class */
                      compute_checksum, /* Name of function in C++  */
                      checksum          /* Arguments */
    );
  }

  void print_val(std::ostream& out, std::string space = "",
                 bool print_decl_p = true) override {
    PYBIND11_OVERLOAD(void,          /* Return type */
                      libdap::Int16, /* Parent class */
                      print_val,     /* Name of function in C++  */
                      out,           /* Arguments */
                      space, print_decl_p);
  }
};

void init_int16(py::module& m) {
  py::class_<libdap::Int16, Int16, libdap::BaseType>(m, "Int16")
      .def(py::init<const std::string&>(), py::arg("n"))
      .def(py::init<const std::string&, const std::string&>(), py::arg("n"),
           py::arg("d"))
      .def("width", &libdap::Int16::width, py::arg("constrained") = false)
      .def("ptr_duplicate", &libdap::Int16::ptr_duplicate)
      // .def("serialize", &libdap::Int16::serialize
      // .def("deserialize", &libdap::Int16::deserialize
      .def("compute_checksum", &libdap::Int16::compute_checksum)
      // .def("serialize", &libdap::Int16::serialize
      // .def("deserialize", &libdap::Int16::deserialize
      .def("set_value", &libdap::Int16::set_value, py::arg("value"))
      .def("value", &libdap::Int16::value)
      .def("print_val",
           [](libdap::Int16& self, std::string& space,
              bool print_decl_p) -> std::string {
             std::ostringstream ss;
             self.print_val(ss, space, print_decl_p);
             return ss.str();
           },
           py::arg("space") = "", py::arg("print_decl_p") = true)
      .def("dump", [](const libdap::Int16& self) -> std::string {
        std::ostringstream ss;
        self.dump(ss);
        return ss.str();
      });
}