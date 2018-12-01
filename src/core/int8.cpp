#include <cstdint>
#include <libdap/crc.h>
#include <libdap/Int8.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sstream>

namespace py = pybind11;

struct Int8 : public libdap::Int8 {
  using libdap::Int8::Int8;

  libdap::BaseType* ptr_duplicate() override {
    PYBIND11_OVERLOAD(libdap::BaseType*, /* Return type */
                      libdap::Int8,      /* Parent class */
                      ptr_duplicate,     /* Name of function in C++  */
    );
  }

  void compute_checksum(Crc32& checksum) override {
    PYBIND11_OVERLOAD(void,             /* Return type */
                      libdap::Int8,     /* Parent class */
                      compute_checksum, /* Name of function in C++  */
                      checksum          /* Arguments */
    );
  }

  void print_val(std::ostream& out, std::string space = "",
                 bool print_decl_p = true) override {
    PYBIND11_OVERLOAD(void,         /* Return type */
                      libdap::Int8, /* Parent class */
                      print_val,    /* Name of function in C++  */
                      out,          /* Arguments */
                      space, print_decl_p);
  }
};

void init_int8(py::module& m) {
  py::class_<libdap::Int8, Int8, libdap::BaseType>(m, "Int8")
      .def(py::init<const std::string&>(), py::arg("n"))
      .def(py::init<const std::string&, const std::string&>(), py::arg("n"),
           py::arg("d"))
      .def("width", &libdap::Int8::width, py::arg("constrained") = false)
      .def("ptr_duplicate", &libdap::Int8::ptr_duplicate)
      // .def("serialize", &libdap::Int8::serialize
      // .def("deserialize", &libdap::Int8::deserialize
      .def("compute_checksum", &libdap::Int8::compute_checksum)
      // .def("serialize", &libdap::Int8::serialize
      // .def("deserialize", &libdap::Int8::deserialize
      .def("set_value", &libdap::Int8::set_value, py::arg("value"))
      .def("value", &libdap::Int8::value)
      .def("print_val",
           [](libdap::Int8& self, std::string& space,
              bool print_decl_p) -> std::string {
             std::ostringstream ss;
             self.print_val(ss, space, print_decl_p);
             return ss.str();
           },
           py::arg("space") = "", py::arg("print_decl_p") = true)
      .def("dump", [](const libdap::Int8& self) -> std::string {
        std::ostringstream ss;
        self.dump(ss);
        return ss.str();
      });
}