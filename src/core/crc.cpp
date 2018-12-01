#include <cstdint>
#include <libdap/crc.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

void init_crc(py::module& m) {
  py::class_<Crc32>(m, "Crc32")
      .def(py::init<>())
      .def("reset", &Crc32::Reset)
      .def("add_data",
           [](Crc32& self, const py::array_t<uint8_t>& data) -> void {
             if (data.ndim() != 1) {
               throw std::invalid_argument(
                   "data must be a one-dimensional array");
             }
             self.AddData(data.data(), data.size());
           },
           py::arg("data"))
      .def("get_crc32", &Crc32::GetCrc32);
}