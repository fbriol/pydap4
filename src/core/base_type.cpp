#include <BaseType.h>
#include <crc.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cstdint>
#include <sstream>

namespace py = pybind11;

struct BaseType : public libdap::BaseType {
  using libdap::BaseType::BaseType;

  libdap::BaseType* ptr_duplicate() override {
    PYBIND11_OVERLOAD_PURE(libdap::BaseType*, /* Return type */
                           libdap::BaseType,  /* Parent class */
                           ptr_duplicate,     /* Name of function in C++  */
    );
  }

  unsigned int buf2val(void** val) override {
    PYBIND11_OVERLOAD_PURE(unsigned int,     /* Return type */
                           libdap::BaseType, /* Parent class */
                           buf2val,          /* Name of function in C++  */
                           val               /* Arguments */
    );
  }

  unsigned int val2buf(void* val, bool reuse = false) override {
    PYBIND11_OVERLOAD_PURE(unsigned int,     /* Return type */
                           libdap::BaseType, /* Parent class */
                           val2buf,          /* Name of function in C++  */
                           val,              /* Arguments */
                           reuse);
  }

  void compute_checksum(Crc32& checksum) override {
    PYBIND11_OVERLOAD_PURE(void,             /* Return type */
                           libdap::BaseType, /* Parent class */
                           compute_checksum, /* Name of function in C++  */
                           checksum          /* Arguments */
    );
  }

  void print_val(std::ostream& out, std::string space = "",
                 bool print_decl_p = true) override {
    PYBIND11_OVERLOAD_PURE(void,             /* Return type */
                           libdap::BaseType, /* Parent class */
                           print_val,        /* Name of function in C++  */
                           out,              /* Arguments */
                           space, print_decl_p);
  }
};

struct BaseTypeStackDeleter {
  template <typename T>
  void operator()(T* self) {
    while (!self->empty()) {
      auto top = self->top();
      delete top;
      self->pop();
    }
  }
};

void init_base_type(py::module& m) {
  py::class_<
      libdap::BaseType::btp_stack,
      std::unique_ptr<libdap::BaseType::btp_stack, BaseTypeStackDeleter>>(
      m, "BaseTypeStack")
      .def(py::init<>())
      .def("empty", &libdap::BaseType::btp_stack::empty)
      .def("__len__", &libdap::BaseType::btp_stack::size)
      .def("top",
           [](libdap::BaseType::btp_stack& self) -> libdap::BaseType* {
             return self.top()->ptr_duplicate();
           })
      .def("pop",
           [](libdap::BaseType::btp_stack& self) -> libdap::BaseType* {
             auto top = self.top();
             auto result = top->ptr_duplicate();
             self.pop();
             delete top;
             return result;
           })
      .def("push", [](libdap::BaseType::btp_stack& self, libdap::BaseType* bt) {
        self.push(bt->ptr_duplicate());
      });

  py::class_<libdap::BaseType, BaseType>(m, "BaseType")
      .def("__str__", &libdap::BaseType::toString)
      // .def("transform_to_dap4", &libdap::BaseType::transform_to_dap4
      // .def("transform_to_dap2", &libdap::BaseType::transform_to_dap2
      .def("dump",
           [](const libdap::BaseType& self) -> std::string {
             std::ostringstream ss;
             self.dump(ss);
             return ss.str();
           })
      .def("clear_local_data", &libdap::BaseType::clear_local_data)
      .def("is_dap4", &libdap::BaseType::is_dap4)
      .def("set_is_dap4", &libdap::BaseType::set_is_dap4, py::arg("v"))
      .def("name", &libdap::BaseType::name)
      .def("set_name", &libdap::BaseType::set_name, py::arg("n"))
      .def("FQN", &libdap::BaseType::FQN)
      .def("type", &libdap::BaseType::type)
      .def("set_type", &libdap::BaseType::set_type, py::arg("type"))
      .def("type_name", &libdap::BaseType::type_name)
      .def("dataset", &libdap::BaseType::dataset)
      .def("length", &libdap::BaseType::length)
      .def("set_length", &libdap::BaseType::set_length, py::arg("length"))
      .def("is_simple_type", &libdap::BaseType::is_simple_type)
      .def("is_vector_type", &libdap::BaseType::is_vector_type)
      .def("is_constructor_type", &libdap::BaseType::is_constructor_type)
      .def("synthesized_p", &libdap::BaseType::synthesized_p)
      .def("set_synthesized_p", &libdap::BaseType::set_synthesized_p,
           py::arg("state"))
      .def("element_count", &libdap::BaseType::element_count,
           py::arg("leaves") = false)
      .def("read_p", &libdap::BaseType::read_p)
      .def("set_read_p", &libdap::BaseType::set_read_p, py::arg("state"))
      .def("send_p", &libdap::BaseType::send_p)
      .def("set_send_p", &libdap::BaseType::set_send_p, py::arg("state"))
      .def("get_attr_table", &libdap::BaseType::get_attr_table)
      .def("set_attr_table", &libdap::BaseType::set_attr_table, py::arg("at"))

      // virtual D4Attributes *attributes();
      // virtual void set_attributes(D4Attributes *);
      // virtual void set_attributes_nocopy(D4Attributes *);

      .def("is_in_selection", &libdap::BaseType::is_in_selection)
      .def("set_in_selection", &libdap::BaseType::set_in_selection,
           py::arg("state"))

      .def("set_parent", &libdap::BaseType::set_parent, py::arg("parent"),
           py::keep_alive<1, 2>())
      .def("get_parent", &libdap::BaseType::get_parent,
           py::return_value_policy::reference_internal)
      .def("transfer_attributes", &libdap::BaseType::transfer_attributes,
           py::arg("at"))
      .def("var",
           [](libdap::BaseType& self, const std::string& name, bool exact_match,
              libdap::BaseType::btp_stack* s) -> libdap::BaseType* {
             return self.var(name, exact_match, s);
           },
           py::arg("name") = "", py::arg("exact_match") = true,
           py::arg("s") = nullptr, py::return_value_policy::reference_internal)
      .def("var",
           [](libdap::BaseType& self, const std::string& name,
              libdap::BaseType::btp_stack& s) -> libdap::BaseType* {
             return self.var(name, s);
           },
           py::arg("name"), py::arg("s"),
           py::return_value_policy::reference_internal)
      .def("read", &libdap::BaseType::read)
      .def("check_semantics", &libdap::BaseType::check_semantics,
           py::arg("msg"), py::arg("all") = false)
      .def("ops", &libdap::BaseType::ops, py::arg("b"), py::arg("op"))
      .def("d4_ops", &libdap::BaseType::ops, py::arg("b"), py::arg("op"))
      .def("width", &libdap::BaseType::width)
      .def("print_decl",
           [](libdap::BaseType& self, std::string space, bool print_semi,
              bool constraint_info, bool constrained) -> std::string {
             std::ostringstream ss;
             self.print_decl(ss, space, print_semi, constraint_info,
                             constrained);
             return ss.str();
           },
           py::arg("space") = "    ", py::arg("print_semi") = true,
           py::arg("constraint_info") = false, py::arg("constrained") = false)
      .def("print_xml",
           [](libdap::BaseType& self, std::string space,
              bool constrained) -> std::string {
             std::ostringstream ss;
             self.print_xml(ss, space, constrained);
             return ss.str();
           },
           py::arg("space") = "    ", py::arg("constrained") = false)
      .def("print_xml_writer", &libdap::BaseType::print_xml_writer,
           py::arg("xml"), py::arg("constrained") = false)
      .def("print_dap4", &libdap::BaseType::print_dap4, py::arg("xml"),
           py::arg("constrained") = false)
      //  .def("intern_data", &libdap::BaseType::intern_data
      //  .def("serialize", &libdap::BaseType::serialize
      .def("intern_data",
           [](libdap::BaseType& self) -> void { self.intern_data(); })
      //  .def("serialize", &libdap::BaseType::serialize
      //  .def("deserialize", &libdap::BaseType::deserialize
      //  .def("deserialize", &libdap::BaseType::deserialize
      ;
}