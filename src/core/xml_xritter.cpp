#include <libdap/XMLWriter.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

void init_xml_writer(py::module& m) {
  py::class_<libdap::XMLWriter>(m, "XMLWriter")
      .def(py::init<const std::string&>(), py::arg("pad") = "    ")
      .def("get_doc", &libdap::XMLWriter::get_doc)
      .def("get_doc_size", &libdap::XMLWriter::get_doc_size);
}
