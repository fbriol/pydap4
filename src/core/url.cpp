#include <cstdint>
#include <libdap/crc.h>
#include <libdap/Url.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

struct Url : public libdap::Url {
  using libdap::Url::Url;

  libdap::BaseType* ptr_duplicate() override {
    PYBIND11_OVERLOAD(libdap::BaseType*, /* Return type */
                      libdap::Url,       /* Parent class */
                      ptr_duplicate,     /* Name of function in C++  */
    );
  }

  void compute_checksum(Crc32& checksum) override {
    PYBIND11_OVERLOAD(void,             /* Return type */
                      libdap::Url,      /* Parent class */
                      compute_checksum, /* Name of function in C++  */
                      checksum          /* Arguments */
    );
  }

  void print_val(std::ostream& out, std::string space = "",
                 bool print_decl_p = true) override {
    PYBIND11_OVERLOAD(void,        /* Return type */
                      libdap::Url, /* Parent class */
                      print_val,   /* Name of function in C++  */
                      out,         /* Arguments */
                      space, print_decl_p);
  }
};

void init_url(py::module& m) {
  py::class_<libdap::Url, Url, libdap::Str>(m, "Url")
      .def(py::init<const std::string&>(), py::arg("n"))
      .def(py::init<const std::string&, const std::string&>(), py::arg("n"),
           py::arg("d"))
      // .def(py::init<const std::string&, libdap::Type>(), py::arg("n"),
      //      py::arg("t"))
      // .def(py::init<const std::string&, const std::string&, libdap::Type>(),
      //      py::arg("n"), py::arg("d"), py::arg("t"));
      .def("ptr_duplicate", &libdap::Url::ptr_duplicate);
}