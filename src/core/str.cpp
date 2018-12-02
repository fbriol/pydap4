#include <Str.h>
#include <crc.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cstdint>
#include <sstream>

namespace py = pybind11;

struct Str : public libdap::Str {
  using libdap::Str::Str;

  libdap::BaseType* ptr_duplicate() override {
    PYBIND11_OVERLOAD(libdap::BaseType*, /* Return type */
                      libdap::Str,       /* Parent class */
                      ptr_duplicate,     /* Name of function in C++  */
    );
  }

  void compute_checksum(Crc32& checksum) override {
    PYBIND11_OVERLOAD(void,             /* Return type */
                      libdap::Str,      /* Parent class */
                      compute_checksum, /* Name of function in C++  */
                      checksum          /* Arguments */
    );
  }

  void print_val(std::ostream& out, std::string space = "",
                 bool print_decl_p = true) override {
    PYBIND11_OVERLOAD(void,        /* Return type */
                      libdap::Str, /* Parent class */
                      print_val,   /* Name of function in C++  */
                      out,         /* Arguments */
                      space, print_decl_p);
  }
};

void init_str(py::module& m) {
  py::class_<libdap::Str, Str, libdap::BaseType>(m, "Str")
      .def(py::init<const std::string&>(), py::arg("n"))
      .def(py::init<const std::string&, const std::string&>(), py::arg("n"),
           py::arg("d"))
      .def("width", &libdap::Str::width, py::arg("constrained") = false)
      .def("length", &libdap::Str::length)
      .def("ptr_duplicate", &libdap::Str::ptr_duplicate)
      // .def("serialize", &libdap::Str::serialize
      // .def("deserialize", &libdap::Str::deserialize
      .def("compute_checksum", &libdap::Str::compute_checksum)
      // .def("serialize", &libdap::Str::serialize
      // .def("deserialize", &libdap::Str::deserialize
      .def("set_value", &libdap::Str::set_value, py::arg("value"))
      .def("value", &libdap::Str::value)
      .def("print_val",
           [](libdap::Str& self, std::string& space,
              bool print_decl_p) -> std::string {
             std::ostringstream ss;
             self.print_val(ss, space, print_decl_p);
             return ss.str();
           },
           py::arg("space") = "", py::arg("print_decl_p") = true)
      .def("dump", [](const libdap::Str& self) -> std::string {
        std::ostringstream ss;
        self.dump(ss);
        return ss.str();
      });
}