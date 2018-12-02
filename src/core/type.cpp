#include <Type.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

void init_type(py::module& m) {
  py::enum_<libdap::Part>(m, "Part")
      .value("nil", libdap::Part::nil)
      .value("array", libdap::Part::array)
      .value("maps", libdap::Part::maps)
      .export_values();

  py::enum_<libdap::Type>(m, "Type")
      .value("dods_null_c", libdap::Type::dods_null_c)
      .value("dods_byte_c", libdap::Type::dods_byte_c)
      .value("dods_int16_c", libdap::Type::dods_int16_c)
      .value("dods_uint16_c", libdap::Type::dods_uint16_c)
      .value("dods_int32_c", libdap::Type::dods_int32_c)
      .value("dods_uint32_c", libdap::Type::dods_uint32_c)
      .value("dods_float32_c", libdap::Type::dods_float32_c)
      .value("dods_float64_c", libdap::Type::dods_float64_c)
      .value("dods_str_c", libdap::Type::dods_str_c)
      .value("dods_url_c", libdap::Type::dods_url_c)
      .value("dods_structure_c", libdap::Type::dods_structure_c)
      .value("dods_array_c", libdap::Type::dods_array_c)
      .value("dods_sequence_c", libdap::Type::dods_sequence_c)
      .value("dods_grid_c", libdap::Type::dods_grid_c)
      .value("dods_char_c", libdap::Type::dods_char_c)
      .value("dods_int8_c", libdap::Type::dods_int8_c)
      .value("dods_uint8_c", libdap::Type::dods_uint8_c)
      .value("dods_int64_c", libdap::Type::dods_int64_c)
      .value("dods_uint64_c", libdap::Type::dods_uint64_c)
      .value("dods_enum_c", libdap::Type::dods_enum_c)
      .value("dods_opaque_c", libdap::Type::dods_opaque_c)
      .value("dods_group_c", libdap::Type::dods_group_c)
      .export_values();
}