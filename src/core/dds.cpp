#include <DDS.h>
#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void init_dds(py::module& m) {
  py::class_<libdap::DDS>(m, "DDS")
      .def(py::init<libdap::BaseTypeFactory*, const std::string&>(),
           py::arg("factory"), py::arg("name") = "", py::keep_alive<1, 2>())
      .def(py::init<libdap::BaseTypeFactory*, const std::string&,
                    const std::string&>(),
           py::arg("factory"), py::arg("name"), py::arg("version"),
           py::keep_alive<1, 2>())
      .def("transfer_attributes", &libdap::DDS::transfer_attributes,
           py::arg("das"))
      .def("get_dataset_name", &libdap::DDS::get_dataset_name)
      .def("set_dataset_name", &libdap::DDS::set_dataset_name, py::arg("n"))
      .def("get_factory", &libdap::DDS::get_factory,
           py::return_value_policy::reference_internal)
      .def("get_attr_table", &libdap::DDS::get_attr_table)
      .def("filename",
           [](const libdap::DDS& self) -> std::string {
             return self.filename();
           })
      .def("filename",
           [](libdap::DDS& self, const std::string& fn) -> void {
             self.filename(fn);
           })
      .def("get_dap_major", &libdap::DDS::get_dap_major)
      .def("get_dap_minor", &libdap::DDS::get_dap_minor)
      .def("set_dap_version",
           [](libdap::DDS& self, const std::string& version_string) -> void {
             self.set_dap_version(version_string);
           },
           py::arg("version") = "2.0")
      .def("get_request_xml_base", &libdap::DDS::get_request_xml_base)
      .def("set_request_xml_base", &libdap::DDS::set_request_xml_base,
           py::arg("xb"))
      .def("get_namespace", &libdap::DDS::get_namespace)
      .def("set_namespace", &libdap::DDS::set_namespace, py::arg("ns"))
      .def("get_response_limit", &libdap::DDS::get_response_limit)
      .def("set_response_limit", &libdap::DDS::set_response_limit,
           py::arg("size"))
      .def("get_request_size", &libdap::DDS::get_request_size)
      .def("container_name",
           [](libdap::DDS& self) -> std::string {
             return self.container_name();
           })
      .def("container_name",
           [](libdap::DDS& self, const std::string& cn) -> void {
             self.container_name(cn);
           },
           py::arg("cn"))
    //     Structure *container() ;
      //
      ;
}