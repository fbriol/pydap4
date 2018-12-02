#include <Connect.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void init_connect(py::module& m) {
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

      // virtual void request_das(DAS &das);
      // virtual void request_das_url(DAS &das);

      // virtual void request_dds(DDS &dds, string expr = "");
      // virtual void request_dds_url(DDS &dds);

      // virtual void request_ddx(DDS &dds, string expr = "");
      // virtual void request_ddx_url(DDS &dds);

      // virtual void request_data(DataDDS &data, string expr = "");
      // virtual void request_data_url(DataDDS &data);

      // virtual void request_data_ddx(DataDDS &data, string expr = "");
      // virtual void request_data_ddx_url(DataDDS &data);

      // virtual void read_data(DataDDS &data, Response *rs);
      // virtual void read_data_no_mime(DataDDS &data, Response *rs);
      // virtual void read_data(DDS &data, Response *rs);
      // virtual void read_data_no_mime(DDS &data, Response *rs);
      ;
}