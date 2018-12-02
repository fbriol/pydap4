#include <Ancillary.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void init_server_ancillary(py::module& m) {
  py::class_<libdap::Ancillary>(m, "Ancillary")
      .def_static("find_ancillary_file",
                  &libdap::Ancillary::find_ancillary_file, py::arg("pathname"),
                  py::arg("ext"), py::arg("dir"), py::arg("file"))
      .def_static("find_group_ancillary_file",
                  &libdap::Ancillary::find_group_ancillary_file,
                  py::arg("pathname"), py::arg("ext"))
      .def_static("read_ancillary_das", &libdap::Ancillary::read_ancillary_das,
                  py::arg("das"), py::arg("pathname"), py::arg("dir") = "",
                  py::arg("file") = "")
      .def_static("read_ancillary_dds", &libdap::Ancillary::read_ancillary_dds,
                  py::arg("dds"), py::arg("pathname"), py::arg("dir") = "",
                  py::arg("file") = "");
}