#include <Array.h>
#include <D4Maps.h>
#include <XMLWriter.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void init_d4_maps(py::module& m) {
  py::class_<libdap::D4Map>(m, "D4Map")
      .def(py::init<>())
      .def(py::init<const std::string&, libdap::Array*, libdap::Array*>(),
           py::arg("name"), py::arg("array"), py::arg("parent") = nullptr,
           py::keep_alive<1, 2>(), py::keep_alive<1, 3>())
      .def("name", &libdap::D4Map::name)
      .def("set_name", &libdap::D4Map::set_name, py::arg("name"))
      .def("array", &libdap::D4Map::array)
      //   .def("set_array", &libdap::D4Map::set_array, py::arg("array"))
      .def("parent", &libdap::D4Map::parent)
      //   .def("set_parent", &libdap::D4Map::set_parent, py::arg("parent"))
      .def("print_dap4", &libdap::D4Map::print_dap4, py::arg("xml"));

  py::class_<libdap::D4Maps>(m, "D4Maps")
      .def(py::init<>())
      .def(py::init<libdap::Array*>(), py::arg("parent"),
           py::keep_alive<1, 2>())
      .def("add_map", &libdap::D4Maps::add_map, py::arg("map"),
           py::keep_alive<1, 2>())
      //   .def("remove_map", &libdap::D4Maps::remove_map, py::arg("map"))
      .def("get_map",
           [](libdap::D4Maps& self, int index) -> libdap::D4Map* {
             if (index > self.size()) {
               throw std::out_of_range("map index out of range");
             }
             return self.get_map(index);
           },
           py::arg("index"), py::return_value_policy::reference_internal)
      .def("__iter__",
           [](libdap::D4Maps& self) {
             return py::make_iterator(
                 self.map_begin(), self.map_end(),
                 py::return_value_policy::reference_internal);
           },
           py::keep_alive<0, 1>())
      .def("size", &libdap::D4Maps::size)
      .def("empty", &libdap::D4Maps::empty)
      .def("print_dap4", &libdap::D4Maps::print_dap4, py::arg("xml"));
}