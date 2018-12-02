#include <BaseType.h>
#include <D4RValue.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sstream>

namespace py = pybind11;

void init_d4_rvalue(py::module& m) {
  py::class_<libdap::D4RValueList>(m, "D4RValueList")
      .def(py::init<>())
      .def("add_rvalue", &libdap::D4RValueList::add_rvalue,
           py::keep_alive<1, 2>())
      .def("get_rvalue",
           [](libdap::D4RValueList& self,
              unsigned int index) -> libdap::D4RValue* {
             if (index > self.size()) {
               throw std::out_of_range("value index out of range");
             }
             return self.get_rvalue(index);
           },
           py::arg("index"), py::return_value_policy::reference_internal)
      .def("size", &libdap::D4RValueList::size)
      .def("__iter__",
           [](libdap::D4RValueList& self) {
             return py::make_iterator(
                 self.begin(), self.end(),
                 py::return_value_policy::reference_internal);
           },
           py::keep_alive<0, 1>());

  py::class_<libdap::D4RValue> d4_rvalue(m, "D4RValue");
  py::enum_<libdap::D4RValue::value_kind>(d4_rvalue, "value_kind")
      .value("unknown", libdap::D4RValue::value_kind::unknown)
      .value("basetype", libdap::D4RValue::value_kind::basetype)
      .value("function", libdap::D4RValue::value_kind::function)
      .value("constan", libdap::D4RValue::value_kind::constant)
      .export_values();

  d4_rvalue.def(py::init<>())
      .def(py::init<libdap::BaseType*>(), py::arg("btp"),
           py::keep_alive<1, 2>())
      // .def(py::init<unsigned long long>(), py::arg("ui"))
      .def(py::init<long long>(), py::arg("i"))
      .def(py::init<double>(), py::arg("r"))
      .def(py::init<std::string>(), py::arg("s"))
      .def(py::init([](py::array_t<uint8_t>& byte_args) {
        auto args = std::vector<libdap::dods_byte>{
            byte_args.data(), byte_args.data() + byte_args.size()};
        return libdap::D4RValue(args);
      }))
      .def(py::init([](py::array_t<uint8_t>& byte_args) {
             auto args = std::vector<libdap::dods_byte>{
                 byte_args.data(), byte_args.data() + byte_args.size()};
             return libdap::D4RValue(args);
           }),
           py::arg("byte_args"))
      .def(py::init([](py::array_t<int8_t>& byte_int8) {
             auto args = std::vector<libdap::dods_int8>{
                 byte_int8.data(), byte_int8.data() + byte_int8.size()};
             return libdap::D4RValue(args);
           }),
           py::arg("byte_int8"))
      .def(py::init([](py::array_t<uint16_t>& byte_uint16) {
             auto args = std::vector<libdap::dods_uint16>{
                 byte_uint16.data(), byte_uint16.data() + byte_uint16.size()};
             return libdap::D4RValue(args);
           }),
           py::arg("byte_uint16"))
      .def(py::init([](py::array_t<int16_t>& byte_int16) {
             auto args = std::vector<libdap::dods_int16>{
                 byte_int16.data(), byte_int16.data() + byte_int16.size()};
             return libdap::D4RValue(args);
           }),
           py::arg("byte_int16"))
      .def(py::init([](py::array_t<uint32_t>& byte_uint32) {
             auto args = std::vector<libdap::dods_uint32>{
                 byte_uint32.data(), byte_uint32.data() + byte_uint32.size()};
             return libdap::D4RValue(args);
           }),
           py::arg("byte_uint32"))
      .def(py::init([](py::array_t<int32_t>& byte_int32) {
             auto args = std::vector<libdap::dods_int32>{
                 byte_int32.data(), byte_int32.data() + byte_int32.size()};
             return libdap::D4RValue(args);
           }),
           py::arg("byte_int32"))
      .def(py::init([](py::array_t<uint64_t>& byte_uint64) {
             auto args = std::vector<libdap::dods_uint64>{
                 byte_uint64.data(), byte_uint64.data() + byte_uint64.size()};
             return libdap::D4RValue(args);
           }),
           py::arg("byte_uint64"))
      .def(py::init([](py::array_t<int64_t>& byte_int64) {
             auto args = std::vector<libdap::dods_int64>{
                 byte_int64.data(), byte_int64.data() + byte_int64.size()};
             return libdap::D4RValue(args);
           }),
           py::arg("byte_int64"))
      .def(py::init([](py::array_t<float>& byte_float32) {
             auto args = std::vector<libdap::dods_float32>{
                 byte_float32.data(),
                 byte_float32.data() + byte_float32.size()};
             return libdap::D4RValue(args);
           }),
           py::arg("byte_float32"))
      .def(py::init([](py::array_t<double>& byte_float64) {
             auto args = std::vector<libdap::dods_float64>{
                 byte_float64.data(),
                 byte_float64.data() + byte_float64.size()};
             return libdap::D4RValue(args);
           }),
           py::arg("byte_float64"))
      .def("get_kind", &libdap::D4RValue::get_kind)
      // .def("value", &libdap::D4RValue::value)
      // .def("value", &libdap::D4RValue::value)
      ;
}