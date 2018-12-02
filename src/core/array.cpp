#include <Array.h>
#include <D4Dimensions.h>
#include <D4Maps.h>
#include <crc.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cstdint>
#include <sstream>

namespace py = pybind11;

struct Array : public libdap::Array {
  using libdap::Array::Array;

  libdap::BaseType* ptr_duplicate() override {
    PYBIND11_OVERLOAD(libdap::BaseType*, /* Return type */
                      libdap::Array,     /* Parent class */
                      ptr_duplicate,     /* Name of function in C++  */
    );
  }

  void compute_checksum(Crc32& checksum) override {
    PYBIND11_OVERLOAD(void,             /* Return type */
                      libdap::Array,    /* Parent class */
                      compute_checksum, /* Name of function in C++  */
                      checksum          /* Arguments */
    );
  }

  void print_val(std::ostream& out, std::string space = "",
                 bool print_decl_p = true) override {
    PYBIND11_OVERLOAD(void,          /* Return type */
                      libdap::Array, /* Parent class */
                      print_val,     /* Name of function in C++  */
                      out,           /* Arguments */
                      space, print_decl_p);
  }
};

static inline libdap::Array::Dim_iter get_iter(libdap::Array& self,
                                               libdap::D4Dimension* dim) {
  for (auto it = self.dim_begin(); it != self.dim_end(); ++it) {
    if (it->dim == dim) {
      return it;
    }
  }
  throw std::invalid_argument(
      "The requested dimension does not belong to this array");
}

void init_array(py::module& m) {
  py::class_<libdap::Array, Array, libdap::Vector>(m, "Array")
      .def(py::init<const std::string&, libdap::BaseType*, bool>(),
           py::arg("n"), py::arg("v"), py::arg("is_dap4") = false)
      .def(py::init<const std::string&, const std::string&, libdap::BaseType*,
                    bool>(),
           py::arg("n"), py::arg("d"), py::arg("v"), py::arg("is_dap4") = false)
      .def("ptr_duplicate", &libdap::Array::ptr_duplicate)
      //   .def("transform_to_dap4", libdap::Array::transform_to_dap4
      //   .def("transform_to_dap4", libdap::Array::transform_to_dap2
      .def("add_var", &libdap::Array::add_var, py::arg("v"),
           py::arg("p") = libdap::Part::nil)
      .def("add_var_nocopy", &libdap::Array::add_var_nocopy, py::arg("v"),
           py::arg("p") = libdap::Part::nil, py::keep_alive<1, 2>())
      .def("append_dim",
           [](libdap::Array& self, int size, const std::string& name) -> void {
             self.append_dim(size, name);
           },
           py::arg("size"), py::arg("name") = "")
      .def("append_dim",
           [](libdap::Array& self, libdap::D4Dimension* dim) -> void {
             self.append_dim(dim);
           },
           py::arg("dim"), py::keep_alive<1, 2>())
      .def("prepend_dim",
           [](libdap::Array& self, int size, const std::string& name) -> void {
             self.prepend_dim(size, name);
           },
           py::arg("size"), py::arg("name") = "")
      .def("prepend_dim",
           [](libdap::Array& self, libdap::D4Dimension* dim) -> void {
             self.prepend_dim(dim);
           },
           py::arg("dim"), py::keep_alive<1, 2>())
      .def("clear_all_dims", &libdap::Array::clear_all_dims)

      .def("add_constraint",
           [](libdap::Array& self, libdap::D4Dimension* where, int start,
              int stride, int stop) -> void {
             auto it = get_iter(self, where);
             self.add_constraint(it, start, stride, stop);
           },
           py::arg("where"), py::arg("start"), py::arg("stride"),
           py::arg("stop"))
      .def("add_constraint",
           [](libdap::Array& self, libdap::D4Dimension* where,
              libdap::D4Dimension* dim) -> void {
             auto it = get_iter(self, where);
             self.add_constraint(it, dim);
           },
           py::arg("where"), py::arg("dim"), py::keep_alive<1, 3>())
      .def("resetconstraint", &libdap::Array::reset_constraint)
      .def("dimension_size",
           [](libdap::Array& self, libdap::D4Dimension* where,
              bool constrained) -> int {
             auto it = get_iter(self, where);
             return self.dimension_size(it, constrained);
           },
           py::arg("where"), py::arg("constrained") = false)
      .def("dimension_start",
           [](libdap::Array& self, libdap::D4Dimension* where,
              bool constrained) -> int {
             auto it = get_iter(self, where);
             return self.dimension_start(it, constrained);
           },
           py::arg("where"), py::arg("constrained") = false)
      .def("dimension_stop",
           [](libdap::Array& self, libdap::D4Dimension* where,
              bool constrained) -> int {
             auto it = get_iter(self, where);
             return self.dimension_stop(it, constrained);
           },
           py::arg("where"), py::arg("constrained") = false)
      .def("dimension_stride",
           [](libdap::Array& self, libdap::D4Dimension* where,
              bool constrained) -> int {
             auto it = get_iter(self, where);
             return self.dimension_stride(it, constrained);
           },
           py::arg("where"), py::arg("constrained") = false)
      .def("dimension_name",
           [](libdap::Array& self, libdap::D4Dimension* where) -> std::string {
             auto it = get_iter(self, where);
             return self.dimension_name(it);
           },
           py::arg("where"))
      .def("dimension",
           [](libdap::Array& self,
              libdap::D4Dimension* where) -> libdap::D4Dimension* {
             auto it = get_iter(self, where);
             return it->dim;
           },
           py::arg("where"), py::return_value_policy::reference_internal)
      .def("dimensions", &libdap::Array::dimensions,
           py::arg("constrained") = false)
      .def("maps", &libdap::Array::maps)
      .def("print_dap4", &libdap::Array::print_dap4, py::arg("xml"),
           py::arg("constrained") = false)
      .def("print_decl",
           [](libdap::Array& self, std::string space, bool print_semi,
              bool constraint_info, bool constrained) -> std::string {
             std::ostringstream ss;
             self.print_decl(ss, space, print_semi, constraint_info,
                             constrained);
             return ss.str();
           },
           py::arg("space") = "    ", py::arg("print_semi") = true,
           py::arg("constraint_info") = false, py::arg("constrained") = false)
      .def("print_xml",
           [](libdap::Array& self, std::string space,
              bool constrained) -> std::string {
             std::ostringstream ss;
             self.print_xml(ss, space, constrained);
             return ss.str();
           },
           py::arg("space") = "    ", py::arg("constrained") = false)
      .def("print_xml_writer", &libdap::Array::print_xml_writer, py::arg("xml"),
           py::arg("constrained") = false)
      .def("print_xml_writer_core", &libdap::Array::print_xml_writer_core,
           py::arg("xml"), py::arg("constrained"), py::arg("tag"))
      .def("print_as_map_xml_writer", &libdap::Array::print_as_map_xml_writer,
           py::arg("xml"), py::arg("constrained"))
      .def("print_val",
           [](libdap::Array& self, std::string space,
              bool print_decl_p) -> std::string {
             std::ostringstream ss;
             self.print_val(ss, space, print_decl_p);
             return ss.str();
           },
           py::arg("space") = "", py::arg("print_decl_p") = true)
      .def("check_semantics", &libdap::Array::check_semantics, py::arg("msg"),
           py::arg("all") = false)
      .def("dump", [](libdap::Array& self) -> std::string {
        std::ostringstream ss;
        self.dump(ss);
        return ss.str();
      });
}