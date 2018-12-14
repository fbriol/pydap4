#include <ObjectType.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

void init_object_type(py::module& m) {
  py::enum_<libdap::ObjectType>(m, "ObjectType")
      .value("unknown_type", libdap::ObjectType::unknown_type)
      .value("dods_das", libdap::ObjectType::dods_das)
      .value("dods_dds", libdap::ObjectType::dods_dds)
      .value("dods_data", libdap::ObjectType::dods_data)
      .value("dods_ddx", libdap::ObjectType::dods_ddx)
      .value("dods_data_ddx", libdap::ObjectType::dods_data_ddx)
      .value("dods_error", libdap::ObjectType::dods_error)
      .value("web_error", libdap::ObjectType::web_error)
      .value("dap4_dmr", libdap::ObjectType::dap4_dmr)
      .value("dap4_data", libdap::ObjectType::dap4_data)
      .value("dap4_error", libdap::ObjectType::dap4_error)
      .export_values();
}