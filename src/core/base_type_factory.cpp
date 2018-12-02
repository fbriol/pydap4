#include <BaseType.h>
#include <BaseTypeFactory.h>
#include <Byte.h>
#include <Float32.h>
#include <Float64.h>
#include <Grid.h>
#include <Int16.h>
#include <Int32.h>
#include <Int64.h>
#include <Int8.h>
#include <Sequence.h>
#include <Str.h>
#include <Structure.h>
#include <UInt16.h>
#include <UInt32.h>
#include <UInt64.h>
#include <Url.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void init_base_type_factory(py::module& m) {
  py::class_<libdap::BaseTypeFactory>(m, "BaseTypeFactory")
      .def(py::init<>())
      .def("new_variable",
           [](const libdap::BaseTypeFactory& self, libdap::Type t,
              const std::string& name) -> libdap::BaseType* {
             return self.NewVariable(t, name);
           },
           py::arg("type"), py::arg("name") = "")
      .def("ptr_duplicate", &libdap::BaseTypeFactory::ptr_duplicate)
      .def("new_byte", &libdap::BaseTypeFactory::NewByte, py::arg("n") = "")
      .def("new_int16", &libdap::BaseTypeFactory::NewInt16, py::arg("n") = "")
      .def("new_uint16", &libdap::BaseTypeFactory::NewUInt16, py::arg("n") = "")
      .def("new_int32", &libdap::BaseTypeFactory::NewInt32, py::arg("n") = "")
      .def("new_uint32", &libdap::BaseTypeFactory::NewUInt32, py::arg("n") = "")
      .def("new_float32", &libdap::BaseTypeFactory::NewFloat32,
           py::arg("n") = "")
      .def("new_float64", &libdap::BaseTypeFactory::NewFloat64,
           py::arg("n") = "")
      .def("new_str", &libdap::BaseTypeFactory::NewStr, py::arg("n") = "")
      .def("new_url", &libdap::BaseTypeFactory::NewUrl, py::arg("n") = "")
      .def("new_array", &libdap::BaseTypeFactory::NewArray, py::arg("n") = "",
           py::arg("v") = nullptr)
      .def("new_structure", &libdap::BaseTypeFactory::NewStructure,
           py::arg("n") = "")
      .def("new_sequence", &libdap::BaseTypeFactory::NewSequence,
           py::arg("n") = "")
      .def("new_grid", &libdap::BaseTypeFactory::NewGrid, py::arg("n") = "");
}