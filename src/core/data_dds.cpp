#include <DataDDS.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sstream>

namespace py = pybind11;

void init_data_dds(py::module& m) {
  py::class_<libdap::DataDDS, libdap::DDS>(m, "DataDDS")
      .def(py::init<libdap::BaseTypeFactory*, const std::string&,
                    const std::string&, const std::string&>(),
           py::arg("factory"), py::arg("name") = "", py::arg("version") = "",
           py::arg("protocol") = "", py::keep_alive<1, 2>())
      .def("set_version", &libdap::DataDDS::set_version, py::arg("version"))
      .def("get_version", &libdap::DataDDS::get_version)
      .def("get_version_major", &libdap::DataDDS::get_version_major)
      .def("get_version_minor", &libdap::DataDDS::get_version_minor)
      .def("set_protocol", &libdap::DataDDS::set_protocol, py::arg("protocol"))
      .def("get_protocol", &libdap::DataDDS::get_protocol)
      .def("get_protocol_major", &libdap::DataDDS::get_protocol_major)
      .def("get_protocol_minor", &libdap::DataDDS::get_protocol_minor)
      .def("dump", [](libdap::DataDDS& self) -> std::string {
        std::ostringstream ss;
        self.dump(ss);
        return ss.str();
      });
}