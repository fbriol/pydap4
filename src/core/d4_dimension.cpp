#include <D4Dimensions.h>
#include <D4Group.h>
#include <XMLWriter.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

static inline libdap::D4Dimensions::D4DimensionsIter get_iter(
    libdap::D4Dimensions& self, const libdap::D4Dimension* dim) {
  for (auto it = self.dim_begin(); it != self.dim_end(); ++it) {
    if ((*it)->name() == dim->name()) {
      return it;
    }
  }
  throw std::invalid_argument(
      "The requested dimension does not belong to this instance");
}

void init_d4_dimensions(py::module& m) {
  py::class_<libdap::D4Dimension>(m, "D4Dimension")
      .def(py::init<>())
      .def(py::init<const std::string&, unsigned long, libdap::D4Dimensions*>(),
           py::arg("name"), py::arg("size"), py::arg("d") = nullptr,
           py::keep_alive<1, 4>())
      .def("name", &libdap::D4Dimension::name)
      .def("set_name", &libdap::D4Dimension::set_name, py::arg("name"))
      .def("fully_qualified_name", &libdap::D4Dimension::fully_qualified_name)
      .def("size", &libdap::D4Dimension::size)
      .def("set_size",
           [](libdap::D4Dimension& self, unsigned long size) -> void {
             self.set_size(size);
           },
           py::arg("size"))
      .def("set_size",
           [](libdap::D4Dimension& self, const std::string& size) -> void {
             self.set_size(size);
           },
           py::arg("size"))
      .def("parent", &libdap::D4Dimension::parent,
           py::return_value_policy::reference_internal)
      .def("set_parent",
           [](libdap::D4Dimension& self, libdap::D4Dimensions* d) -> void {
             self.set_parent(d);
           },
           py::arg("d"), py::keep_alive<1, 2>())
      .def("constrained", &libdap::D4Dimension::constrained)
      .def("c_start", &libdap::D4Dimension::c_start)
      .def("c_stride", &libdap::D4Dimension::c_stride)
      .def("c_stop", &libdap::D4Dimension::c_stop)
      .def("used_by_projected_var", &libdap::D4Dimension::used_by_projected_var)
      .def("set_used_by_projected_var",
           &libdap::D4Dimension::set_used_by_projected_var, py::arg("state"))
      .def("set_constraint", &libdap::D4Dimension::set_constraint,
           py::arg("start"), py::arg("stride"), py::arg("stop"))
      .def("print_dap4", &libdap::D4Dimension::print_dap4, py::arg("xml"));

  py::class_<libdap::D4Dimensions>(m, "D4Dimensions")
      .def(py::init<>())
      .def(py::init<libdap::D4Group*>(), py::arg("g"), py::keep_alive<1, 2>())
      .def("empty", &libdap::D4Dimensions::empty)
      .def("parent", &libdap::D4Dimensions::parent,
           py::return_value_policy::reference_internal)
      .def("set_parent",
           [](libdap::D4Dimensions& self, libdap::D4Group* g) -> void {
             self.set_parent(g);
           },
           py::arg("g"), py::keep_alive<1, 2>())
      .def("add_dim", &libdap::D4Dimensions::add_dim, py::arg("dim"))
      .def("add_dim_nocopy", &libdap::D4Dimensions::add_dim_nocopy,
           py::arg("dim"), py::keep_alive<1, 2>())
      .def("__iter__",
           [](libdap::D4Dimensions& self) {
             return py::make_iterator(
                 self.dim_begin(), self.dim_end(),
                 py::return_value_policy::reference_internal);
           },
           py::keep_alive<0, 1>())
      .def("find_dim", &libdap::D4Dimensions::find_dim, py::arg("name"),
           py::return_value_policy::reference_internal)
      .def("insert_dim",
           [](libdap::D4Dimensions& self, libdap::D4Dimension* dim,
              libdap::D4Dimension* before) -> void {
             self.insert_dim(dim, get_iter(self, before));
           },
           py::arg("dim"), py::arg("before"))
      .def("insert_dim_nocopy",
           [](libdap::D4Dimensions& self, libdap::D4Dimension* dim,
              libdap::D4Dimension* before) -> void {
             self.insert_dim_nocopy(dim, get_iter(self, before));
           },
           py::arg("dim"), py::arg("before"), py::keep_alive<1, 2>())
      .def("print_dap4", &libdap::D4Dimensions::print_dap4, py::arg("xml"),
           py::arg("constrained") = false);
}