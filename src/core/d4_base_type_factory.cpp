#include <BaseType.h>
#include <Byte.h>
#include <D4BaseTypeFactory.h>
#include <D4Enum.h>
#include <D4Group.h>
#include <D4Opaque.h>
#include <D4Sequence.h>
#include <Float32.h>
#include <Float64.h>
#include <Grid.h>
#include <Int16.h>
#include <Int32.h>
#include <Int64.h>
#include <Int8.h>
#include <Str.h>
#include <Structure.h>
#include <UInt16.h>
#include <UInt32.h>
#include <UInt64.h>
#include <Url.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void init_d4_base_type_factory(py::module& m) {
  py::class_<libdap::D4BaseTypeFactory>(m, "D4BaseTypeFactory")
      .def(py::init<>())
      .def("new_variable",
           [](const libdap::D4BaseTypeFactory& self, libdap::Type t,
              const std::string& name) -> libdap::BaseType* {
             return self.NewVariable(t, name);
           },
           py::arg("type"), py::arg("name") = "")
      .def("ptr_duplicate", &libdap::D4BaseTypeFactory::ptr_duplicate)
      .def("new_byte", &libdap::D4BaseTypeFactory::NewByte, py::arg("n") = "")
      .def("new_int8", &libdap::D4BaseTypeFactory::NewInt8, py::arg("n") = "")
      .def("new_char", &libdap::D4BaseTypeFactory::NewChar, py::arg("n") = "")
      .def("new_uint8", &libdap::D4BaseTypeFactory::NewUInt8, py::arg("n") = "")
      .def("new_int16", &libdap::D4BaseTypeFactory::NewInt16, py::arg("n") = "")
      .def("new_uint16", &libdap::D4BaseTypeFactory::NewUInt16,
           py::arg("n") = "")
      .def("new_int32", &libdap::D4BaseTypeFactory::NewInt32, py::arg("n") = "")
      .def("new_uint32", &libdap::D4BaseTypeFactory::NewUInt32,
           py::arg("n") = "")
      .def("new_int64", &libdap::D4BaseTypeFactory::NewInt64, py::arg("n") = "")
      .def("new_uint64", &libdap::D4BaseTypeFactory::NewUInt64,
           py::arg("n") = "")
      .def("new_float32", &libdap::D4BaseTypeFactory::NewFloat32,
           py::arg("n") = "")
      .def("new_float64", &libdap::D4BaseTypeFactory::NewFloat64,
           py::arg("n") = "")
      .def("new_enum", &libdap::D4BaseTypeFactory::NewEnum, py::arg("n") = "",
           py::arg("type") = libdap::dods_null_c)
      .def("new_str", &libdap::D4BaseTypeFactory::NewStr, py::arg("n") = "")
      .def("new_url", &libdap::D4BaseTypeFactory::NewUrl, py::arg("n") = "")
      .def("new_opaque", &libdap::D4BaseTypeFactory::NewOpaque,
           py::arg("n") = "")
      .def("new_array", &libdap::D4BaseTypeFactory::NewArray, py::arg("n") = "",
           py::arg("v") = nullptr)
      .def("new_structure", &libdap::D4BaseTypeFactory::NewStructure,
           py::arg("n") = "")
      .def("new_d4sequence", &libdap::D4BaseTypeFactory::NewD4Sequence,
           py::arg("n") = "")
      .def("new_group", &libdap::D4BaseTypeFactory::NewGroup,
           py::arg("n") = "");
}