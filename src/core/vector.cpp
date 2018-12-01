#include <cstdint>
#include <libdap/crc.h>
#include <libdap/Vector.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace py = pybind11;

struct Vector : public libdap::Vector {
  using libdap::Vector::Vector;

  libdap::BaseType* ptr_duplicate() override {
    PYBIND11_OVERLOAD_PURE(libdap::BaseType*, /* Return type */
                           libdap::Vector,    /* Parent class */
                           ptr_duplicate,     /* Name of function in C++  */
    );
  }

  void compute_checksum(Crc32& checksum) override {
    PYBIND11_OVERLOAD(void,             /* Return type */
                      libdap::Vector,   /* Parent class */
                      compute_checksum, /* Name of function in C++  */
                      checksum          /* Arguments */
    );
  }

  void print_val(std::ostream& out, std::string space = "",
                 bool print_decl_p = true) override {
    PYBIND11_OVERLOAD_PURE(void,           /* Return type */
                           libdap::Vector, /* Parent class */
                           print_val,      /* Name of function in C++  */
                           out,            /* Arguments */
                           space, print_decl_p);
  }
};

void init_vector(py::module& m) {
  py::class_<libdap::Vector, Vector, libdap::BaseType>(m, "Vector")
      .def(py::init<const std::string&, libdap::BaseType*, const libdap::Type&,
                    bool>(),
           py::arg("n"), py::arg("v"), py::arg("t"), py::arg("is_dap4") = false)
      .def(py::init<const std::string&, const std::string&, libdap::BaseType*,
                    const libdap::Type&, bool>(),
           py::arg("n"), py::arg("d"), py::arg("v"), py::arg("t"),
           py::arg("is_dap4") = false)
      .def("get_str", &libdap::Vector::get_str)
      .def("prototype", &libdap::Vector::prototype,
           py::return_value_policy::reference_internal)
      .def("set_name", &libdap::Vector::set_name, py::arg("name"))
      .def("element_count", &libdap::Vector::element_count, py::arg("leaves"))
      .def("set_send_p", &libdap::Vector::set_send_p, py::arg("state"))
      .def("set_read_p", &libdap::Vector::set_read_p, py::arg("state"))
      .def("width", &libdap::Vector::width, py::arg("constrained") = false)
      .def("length", &libdap::Vector::length)
      .def("set_length", &libdap::Vector::set_length, py::arg("l"))
      // .def("intern_data", &libdap::Vector::intern_data
      // .def("serialize", &libdap::Vector::serialize
      // .def("deserialize", &libdap::Vector::deserialize
      .def("compute_checksum", &libdap::Vector::compute_checksum)
      // .def("intern_data", &libdap::Vector::intern_data
      // .def("serialize", &libdap::Vector::serialize
      // .def("deserialize", &libdap::Vector::deserialize
      .def("set_vec", &libdap::Vector::set_vec, py::arg("i"), py::arg("val"))
      .def("set_vec_nocopy", &libdap::Vector::set_vec_nocopy, py::arg("i"),
           py::arg("val"), py::keep_alive<1, 3>())
      .def("vec_resize", &libdap::Vector::vec_resize, py::arg("l"))
      .def("clear_local_data", &libdap::Vector::clear_local_data)
      .def("get_value_capacity", &libdap::Vector::get_value_capacity)
      .def("reserve_value_capacity",
           [](libdap::Vector& self, unsigned int num) -> void {
             self.reserve_value_capacity(num);
           },
           py::arg("num_elements"))
      .def("reserve_value_capacity",
           [](libdap::Vector& self) -> void { self.reserve_value_capacity(); })
      .def("clear_local_data", &libdap::Vector::clear_local_data)
      .def("set_value_slice_from_row_major_vector",
           &libdap::Vector::set_value_slice_from_row_major_vector,
           py::arg("row_major_data"), py::arg("start_element"))
      .def("set_value",
           [](libdap::Vector& self,
              py::array_t<libdap::dods_byte>& val) -> bool {
             if (val.ndim() != 1) {
               throw std::invalid_argument(
                   "val must be a one-dimensional array");
             }
             return self.set_value(val.mutable_data(),
                                   static_cast<int>(val.size()));
           },
           py::arg("val"))
      .def("set_value",
           [](libdap::Vector& self,
              py::array_t<libdap::dods_int8>& val) -> bool {
             if (val.ndim() != 1) {
               throw std::invalid_argument(
                   "val must be a one-dimensional array");
             }
             return self.set_value(val.mutable_data(),
                                   static_cast<int>(val.size()));
           },
           py::arg("val"))
      .def("set_value",
           [](libdap::Vector& self,
              py::array_t<libdap::dods_int16>& val) -> bool {
             if (val.ndim() != 1) {
               throw std::invalid_argument(
                   "val must be a one-dimensional array");
             }
             return self.set_value(val.mutable_data(),
                                   static_cast<int>(val.size()));
           },
           py::arg("val"))
      .def("set_value",
           [](libdap::Vector& self,
              py::array_t<libdap::dods_uint16>& val) -> bool {
             if (val.ndim() != 1) {
               throw std::invalid_argument(
                   "val must be a one-dimensional array");
             }
             return self.set_value(val.mutable_data(),
                                   static_cast<int>(val.size()));
           },
           py::arg("val"))
      .def("set_value",
           [](libdap::Vector& self,
              py::array_t<libdap::dods_int32>& val) -> bool {
             if (val.ndim() != 1) {
               throw std::invalid_argument(
                   "val must be a one-dimensional array");
             }
             return self.set_value(val.mutable_data(),
                                   static_cast<int>(val.size()));
           },
           py::arg("val"))
      .def("set_value",
           [](libdap::Vector& self,
              py::array_t<libdap::dods_uint32>& val) -> bool {
             if (val.ndim() != 1) {
               throw std::invalid_argument(
                   "val must be a one-dimensional array");
             }
             return self.set_value(val.mutable_data(),
                                   static_cast<int>(val.size()));
           },
           py::arg("val"))
      .def("set_value",
           [](libdap::Vector& self,
              py::array_t<libdap::dods_int64>& val) -> bool {
             if (val.ndim() != 1) {
               throw std::invalid_argument(
                   "val must be a one-dimensional array");
             }
             return self.set_value(val.mutable_data(),
                                   static_cast<int>(val.size()));
           },
           py::arg("val"))
      .def("set_value",
           [](libdap::Vector& self,
              py::array_t<libdap::dods_uint64>& val) -> bool {
             if (val.ndim() != 1) {
               throw std::invalid_argument(
                   "val must be a one-dimensional array");
             }
             return self.set_value(val.mutable_data(),
                                   static_cast<int>(val.size()));
           },
           py::arg("val"))
      .def("set_value",
           [](libdap::Vector& self,
              py::array_t<libdap::dods_float32>& val) -> bool {
             if (val.ndim() != 1) {
               throw std::invalid_argument(
                   "val must be a one-dimensional array");
             }
             return self.set_value(val.mutable_data(),
                                   static_cast<int>(val.size()));
           },
           py::arg("val"))
      .def("set_value",
           [](libdap::Vector& self,
              py::array_t<libdap::dods_float64>& val) -> bool {
             if (val.ndim() != 1) {
               throw std::invalid_argument(
                   "val must be a one-dimensional array");
             }
             return self.set_value(val.mutable_data(),
                                   static_cast<int>(val.size()));
           },
           py::arg("val"))
      .def("set_value",
           [](libdap::Vector& self, std::vector<std::string>& val) -> bool {
             return self.set_value(val, val.size());
           },
           py::arg("val"))
      .def("value",
           [](libdap::Vector& self,
              const pybind11::object& dtype) -> py::object {

#define VALUE(TYPE)                    \
  {                                    \
    auto result = py::array_t<TYPE>(); \
    result.resize({self.length()});    \
    self.value(result.mutable_data()); \
    return result;                     \
    break;                             \
  }
             auto np_type = pybind11::dtype::from_args(dtype);
             auto num = np_type.attr("num").template cast<int>();
             switch (num) {
               case pybind11::detail::npy_api::NPY_BYTE_:
                 VALUE(libdap::dods_int8)
               case pybind11::detail::npy_api::NPY_UBYTE_:
                 VALUE(libdap::dods_byte)
                 break;
               case pybind11::detail::npy_api::NPY_SHORT_:
                 VALUE(libdap::dods_int16)
                 break;
               case pybind11::detail::npy_api::NPY_USHORT_:
                 VALUE(libdap::dods_uint16)
                 break;
               case pybind11::detail::npy_api::NPY_INT_:
                 VALUE(libdap::dods_int32)
                 break;
               case pybind11::detail::npy_api::NPY_UINT_:
                 VALUE(libdap::dods_uint32)
                 break;
               case pybind11::detail::npy_api::NPY_LONG_:
                 VALUE(libdap::dods_int64)
                 break;
               case pybind11::detail::npy_api::NPY_ULONG_:
                 VALUE(libdap::dods_uint64)
                 break;
               case pybind11::detail::npy_api::NPY_FLOAT_:
                 VALUE(libdap::dods_float32)
                 break;
               case pybind11::detail::npy_api::NPY_DOUBLE_:
                 VALUE(libdap::dods_float64)
                 break;
               default:
                 throw std::runtime_error(
                     "data type \"" +
                     std::string(pybind11::str(np_type.ptr())) +
                     "\" not handled");
             }
           },
           py::arg("dtype"))
      .def("value",
           [](libdap::Vector& self) -> std::vector<std::string> {
             auto result = std::vector<std::string>();
             self.value(result);
             return result;
           })
      .def("value",
           [](libdap::Vector& self, py::array_t<unsigned int>& indices,
              const pybind11::object& dtype) -> py::object {

#define SLICE(TYPE)                          \
  {                                          \
    auto result = py::array_t<TYPE>();       \
    result.resize({arg.size()});             \
    self.value(&arg, result.mutable_data()); \
    return result;                           \
    break;                                   \
  }
             if (indices.ndim() != 1) {
               throw std::invalid_argument(
                   "indices must be a one-dimensional array");
             }
             std::vector<unsigned int> arg(indices.shape(0));
             auto data = indices.mutable_unchecked<1>();
             for (ssize_t ix = 0; ix < indices.shape(0); ix++) {
               arg[ix] = data(ix);
             }
             auto np_type = pybind11::dtype::from_args(dtype);
             auto num = np_type.attr("num").template cast<int>();
             switch (num) {
               case pybind11::detail::npy_api::NPY_BYTE_:
                 SLICE(libdap::dods_int8)
               case pybind11::detail::npy_api::NPY_UBYTE_:
                 SLICE(libdap::dods_byte)
                 break;
               case pybind11::detail::npy_api::NPY_SHORT_:
                 SLICE(libdap::dods_int16)
                 break;
               case pybind11::detail::npy_api::NPY_USHORT_:
                 SLICE(libdap::dods_uint16)
                 break;
               case pybind11::detail::npy_api::NPY_INT_:
                 SLICE(libdap::dods_int32)
                 break;
               case pybind11::detail::npy_api::NPY_UINT_:
                 SLICE(libdap::dods_uint32)
                 break;
               case pybind11::detail::npy_api::NPY_LONG_:
                 SLICE(libdap::dods_int64)
                 break;
               case pybind11::detail::npy_api::NPY_ULONG_:
                 SLICE(libdap::dods_uint64)
                 break;
               case pybind11::detail::npy_api::NPY_FLOAT_:
                 SLICE(libdap::dods_float32)
                 break;
               case pybind11::detail::npy_api::NPY_DOUBLE_:
                 SLICE(libdap::dods_float64)
                 break;
               default:
                 throw std::runtime_error(
                     "data type \"" +
                     std::string(pybind11::str(np_type.ptr())) +
                     "\" not handled");
             }
           },
           py::arg("indices"), py::arg("dtype"))
      //  .def("var",
      //  .def("var",
      //  .def("var",
      .def("dump", [](const libdap::Vector& self) -> std::string {
        std::ostringstream ss;
        self.dump(ss);
        return ss.str();
      });
}
