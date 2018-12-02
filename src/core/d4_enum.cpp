#include <D4Enum.h>
#include <D4EnumDefs.h>
#include <crc.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cstdint>
#include <sstream>

namespace py = pybind11;

struct D4Enum : public libdap::D4Enum {
  using libdap::D4Enum::D4Enum;

  libdap::BaseType *ptr_duplicate() override {
    PYBIND11_OVERLOAD(libdap::BaseType *, /* Return type */
                      libdap::D4Enum,     /* Parent class */
                      ptr_duplicate,      /* Name of function in C++  */
    );
  }

  void compute_checksum(Crc32 &checksum) override {
    PYBIND11_OVERLOAD(void,             /* Return type */
                      libdap::D4Enum,   /* Parent class */
                      compute_checksum, /* Name of function in C++  */
                      checksum          /* Arguments */
    );
  }

  void print_val(std::ostream &out, std::string space = "",
                 bool print_decl_p = true) override {
    PYBIND11_OVERLOAD(void,           /* Return type */
                      libdap::D4Enum, /* Parent class */
                      print_val,      /* Name of function in C++  */
                      out,            /* Arguments */
                      space, print_decl_p);
  }
};

void init_d4_enum(py::module &m) {
  py::class_<libdap::D4Enum, D4Enum, libdap::BaseType>(m, "D4Enum")
      .def(py::init<const std::string &, const std::string &>(),
           py::arg("name"), py::arg("enum_type"))
      .def(py::init<const std::string &, libdap::Type>(), py::arg("name"),
           py::arg("type"))
      .def(py::init<const std::string &, const std::string &, libdap::Type>(),
           py::arg("name"), py::arg("dataset"), py::arg("type"))
      .def("enumeration", &libdap::D4Enum::enumeration,
           py::return_value_policy::reference_internal)
      .def("set_enumeration", &libdap::D4Enum::set_enumeration,
           py::arg("enumeration"), py::keep_alive<1, 2>())
      .def("ptr_duplicate", &libdap::D4Enum::ptr_duplicate)
      .def("element_type", &libdap::D4Enum::element_type)
      .def("set_element_type", &libdap::D4Enum::set_element_type,
           py::arg("type"))
      .def("is_signed", &libdap::D4Enum::is_signed)
      .def("set_is_signed", &libdap::D4Enum::set_is_signed, py::arg("t"))
      .def("value",
           [](libdap::D4Enum &self) -> int64_t {
             int64_t value;
             self.value<int64_t>(&value);
             return value;
           })
      .def("set_value",
           [](libdap::D4Enum &self, int64_t value, bool check_value) -> void {
             self.set_value<int64_t>(value, check_value);
           },
           py::arg("value"), py::arg("check_value"))
      .def("width", &libdap::D4Enum::width)
      .def("compute_checksum", &libdap::D4Enum::compute_checksum,
           py::arg("crc"))
      // .def("serialize", &libdap::D4Enum::serialize
      // .def("deserialize", &libdap::D4Enum::deserialize
      .def("print_val",
           [](libdap::D4Enum &self, std::string space,
              bool print_decl_p) -> std::string {
             std::ostringstream ss;
             self.print_val(ss, space, print_decl_p);
             return ss.str();
           },
           py::arg("space") = "", py::arg("print_decl_p") = true)
      .def("print_xml_writer", &libdap::D4Enum::print_xml_writer,
           py::arg("xml"), py::arg("constrained") = false)
      // .def("ops", &libdap::D4Enum::ops
      .def("dump",
           [](libdap::D4Enum &self) -> std::string {
             std::ostringstream ss;
             self.dump(ss);
             return ss.str();
           })
      // .def("transform_to_dap2", libdap::D4Enum::transform_to_dap2
      ;
}