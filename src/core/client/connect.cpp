#include <Connect.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void init_client_connect(py::module& m) {
  py::class_<libdap::Connect>(m, "Connect")
      .def(py::init<const std::string&, std::string, std::string>(),
           py::arg("name"), py::arg("uname") = "", py::arg("password") = "")
      .def("is_local", &libdap::Connect::is_local)
      .def("URL", &libdap::Connect::URL, py::arg("CE") = true)
      .def("CE", &libdap::Connect::CE)
      .def("set_credentials", &libdap::Connect::set_credentials,
           py::arg("uname"), py::arg("password"))
      .def("set_accept_deflate", &libdap::Connect::set_accept_deflate,
           py::arg("deflate"))
      .def("set_xdap_protocol", &libdap::Connect::set_xdap_protocol,
           py::arg("major"), py::arg("minor"))
      .def("set_cache_enabled", &libdap::Connect::set_cache_enabled,
           py::arg("enabled"))
      .def("is_cache_enabled", &libdap::Connect::is_cache_enabled)
      //  .def("set_xdap_accept",
      //       &libdap::Connect::set_xdap_accept,
      //       py::arg("major"),
      //       py::arg("minor"))
      .def("get_protocol", &libdap::Connect::get_protocol)
      .def("request_version", &libdap::Connect::request_version)
      .def("request_protocol", &libdap::Connect::request_protocol)
      .def("request_das", &libdap::Connect::request_das, py::arg("das"))
      .def("request_das_url", &libdap::Connect::request_das_url, py::arg("das"))
      .def("request_dds", &libdap::Connect::request_dds, py::arg("dds"),
           py::arg("expr"))
      .def("request_dds_url", &libdap::Connect::request_dds_url, py::arg("dds"))
      .def("request_ddx", &libdap::Connect::request_ddx, py::arg("dds"),
           py::arg("expr"))
      .def("request_ddx_url", &libdap::Connect::request_ddx_url, py::arg("dds"))
      .def("request_data", &libdap::Connect::request_data, py::arg("data"),
           py::arg("expr"))
      .def("request_data_url", &libdap::Connect::request_data_url,
           py::arg("data"))
      .def("request_data_ddx", &libdap::Connect::request_data_ddx,
           py::arg("data"), py::arg("expr"))
      .def("request_data_ddx_url", &libdap::Connect::request_data_ddx_url,
           py::arg("data"))
      .def("read_data",
           [](libdap::Connect& self, libdap::DataDDS& data,
              libdap::Response* rs) -> void { self.read_data(data, rs); },
           py::arg("data"), py::arg("response"))
      .def("read_data_no_mime",
           [](libdap::Connect& self, libdap::DataDDS& data,
              libdap::Response* rs) -> void {
             self.read_data_no_mime(data, rs);
           },
           py::arg("data"), py::arg("response"))
      .def("read_data",
           [](libdap::Connect& self, libdap::DDS& data,
              libdap::Response* rs) -> void { self.read_data(data, rs); },
           py::arg("data"), py::arg("response"))
      .def("read_data_no_mime",
           [](libdap::Connect& self, libdap::DDS& data, libdap::Response* rs)
               -> void { self.read_data_no_mime(data, rs); },
           py::arg("data"), py::arg("response"));
}