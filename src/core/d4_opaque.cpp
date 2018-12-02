#include <D4Opaque.h>
#include <crc.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cstdint>
#include <sstream>

namespace py = pybind11;

struct D4Opaque : public libdap::D4Opaque {
  using libdap::D4Opaque::D4Opaque;

  libdap::BaseType* ptr_duplicate() override {
    PYBIND11_OVERLOAD(libdap::BaseType*, /* Return type */
                      libdap::D4Opaque,  /* Parent class */
                      ptr_duplicate,     /* Name of function in C++  */
    );
  }

  void compute_checksum(Crc32& checksum) override {
    PYBIND11_OVERLOAD(void,             /* Return type */
                      libdap::D4Opaque, /* Parent class */
                      compute_checksum, /* Name of function in C++  */
                      checksum          /* Arguments */
    );
  }

  void print_val(std::ostream& out, std::string space = "",
                 bool print_decl_p = true) override {
    PYBIND11_OVERLOAD(void,             /* Return type */
                      libdap::D4Opaque, /* Parent class */
                      print_val,        /* Name of function in C++  */
                      out,              /* Arguments */
                      space, print_decl_p);
  }
};

void init_d4_opaque(py::module& m) {
  py::class_<libdap::D4Opaque, D4Opaque, libdap::BaseType>(m, "D4Opaque")
      .def(py::init<const std::string&>(), py::arg("n"))
      .def(py::init<const std::string&, const std::string&>(), py::arg("n"),
           py::arg("d"))
      .def("ptr_duplicate", &libdap::D4Opaque::ptr_duplicate)
      .def("clear_local_data", &libdap::D4Opaque::clear_local_data)
      .def("width", &libdap::D4Opaque::width)
      .def("length", &libdap::D4Opaque::length)
      // .def("serialize", &libdap::D4Opaque::serialize
      // .def("deserialize", &libdap::D4Opaque::deserialize
      .def("compute_checksum", &libdap::D4Opaque::compute_checksum)
      // .def("serialize", &libdap::D4Opaque::serialize
      // .def("deserialize", &libdap::D4Opaque::deserialize
      .def("set_value",
           [](libdap::D4Opaque& self,
              const py::array_t<uint8_t>& value) -> void {
             if (value.ndim() != 1) {
               throw std::invalid_argument(
                   "value must be a one-dimensional array");
             }
             self.set_value(libdap::D4Opaque::dods_opaque{
                 value.data(), value.data() + value.size()});
           },
           py::arg("value"))
      .def("value",
           [](libdap::D4Opaque& self) -> py::array_t<uint8_t> {
             auto value = self.value();

             py::array_t<uint8_t> result({static_cast<size_t>(self.length())});
             auto ptr = result.mutable_unchecked<1>();
             for (ssize_t ix = 0; ix < self.length(); ++ix) {
               ptr(ix) = value[ix];
             }

             return result;
           })
      .def("print_val",
           [](libdap::D4Opaque& self, std::string& space,
              bool print_decl_p) -> std::string {
             std::ostringstream ss;
             self.print_val(ss, space, print_decl_p);
             return ss.str();
           },
           py::arg("space") = "", py::arg("print_decl_p") = true)
      // .def("transform_to_dap2", &libdap::D4Opaque::transform_to_dap2
      .def("dump", [](const libdap::D4Opaque& self) -> std::string {
        std::ostringstream ss;
        self.dump(ss);
        return ss.str();
      });
}