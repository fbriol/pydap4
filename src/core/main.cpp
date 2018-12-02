#include <pybind11/pybind11.h>
namespace py = pybind11;

void init_array(py::module& m);
void init_attr_table(py::module& m);
void init_base_type(py::module& m);
void init_base_type_factory(py::module& m);
void init_d4_base_type_factory(py::module& m);
void init_byte(py::module& m);
void init_constructor(py::module& m);
void init_crc(py::module& m);
void init_d4_attributes(py::module& m);
void init_d4_dimensions(py::module& m);
void init_d4_enum(py::module& m);
void init_d4_enum_defs(py::module& m);
void init_d4_group(py::module& m);
void init_d4_maps(py::module& m);
void init_d4_opaque(py::module& m);
void init_d4_rvalue(py::module& m);
void init_d4_sequence(py::module& m);
void init_exceptions(py::module& m);
void init_float32(py::module& m);
void init_float64(py::module& m);
void init_grid(py::module& m);
void init_int16(py::module& m);
void init_int32(py::module& m);
void init_int64(py::module& m);
void init_int8(py::module& m);
void init_type(py::module& m);
void init_str(py::module& m);
void init_url(py::module& m);
void init_uint16(py::module& m);
void init_uint32(py::module& m);
void init_uint64(py::module& m);
void init_vector(py::module& m);
void init_xml_writer(py::module& m);

//
void init_server_ancillary(py::module& m);

PYBIND11_MODULE(core, m) {
  auto client = m.def_submodule("client");
  auto server = m.def_submodule("server");

  // Core
  init_exceptions(m);
  init_crc(m);
  init_type(m);
  init_base_type(m);
  init_constructor(m);
  init_int8(m);
  init_int16(m);
  init_int32(m);
  init_int64(m);
  init_byte(m);
  init_uint16(m);
  init_uint32(m);
  init_uint64(m);
  init_float32(m);
  init_float64(m);
  init_str(m);
  init_url(m);

  init_d4_attributes(m);
  init_d4_dimensions(m);
  init_d4_enum(m);
  init_d4_enum_defs(m);
  init_d4_group(m);
  init_d4_maps(m);
  init_d4_opaque(m);
  init_d4_rvalue(m);
  init_d4_sequence(m);

  init_vector(m);
  init_array(m);
  init_grid(m);

  init_base_type_factory(m);
  init_d4_base_type_factory(m);

  init_attr_table(m);

  init_xml_writer(m);

  // Server
  init_server_ancillary(server);
}
