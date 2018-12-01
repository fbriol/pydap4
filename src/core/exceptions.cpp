#include <libdap/InternalErr.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sstream>

namespace py = pybind11;

// Helper function to throw a Python exception when exception translation fails.
void raise_exception_failed(const char *message) {
  PyErr_SetString(PyExc_RuntimeError, message);
}

// Code hack from LSSTApplications
void raise_exception(py::object &exc) {
  static auto module = py::reinterpret_borrow<py::object>(
      PyImport_ImportModule("pydap4.exceptions"));
  if (module.ptr() == nullptr) {
    raise_exception_failed("Failed to import C++ Exception wrapper module.");
  } else {
    static auto translate = py::reinterpret_borrow<py::object>(
        PyObject_GetAttrString(module.ptr(), "translate"));
    if (translate.ptr() == nullptr) {
      raise_exception_failed(
          "Failed to find translation function for C++ Exceptions.");
    } else {
      // Calling the Python translate() returns an instance of the appropriate
      // Python exception that wraps the C++ exception instance that we give it.
      auto instance = py::reinterpret_steal<py::object>(
          PyObject_CallFunctionObjArgs(translate.ptr(), exc.ptr(), NULL));

      if (!instance.ptr()) {
        // We actually expect a null return here, as translate() should raise an
        // exception
        raise_exception_failed("Failed to translate C++ Exception to Python.");
      } else {
        auto type =
            py::reinterpret_borrow<py::object>(PyObject_Type(instance.ptr()));
        PyErr_SetObject(type.ptr(), instance.ptr());
      }
    }
  }
}

void init_exceptions(py::module &m) {
  py::class_<libdap::Error>(m, "Error")
      .def(py::init<int, std::string>(), py::arg("ec"), py::arg("msg"))
      .def(py::init<std::string>(), py::arg("msg"))
      .def(py::init<>())
      .def("OK", &libdap::Error::OK)
      .def("parse",
           [](libdap::Error &self, const std::string &fname) -> bool {
             auto fp = std::unique_ptr<FILE, int (*)(FILE *)>(
                 fopen(fname.c_str(), "r"), [](FILE *fp) -> int {
                   if (fp != nullptr) {
                     return fclose(fp);
                   }
                   return 0;
                 });
             if (fp.get() == nullptr) {
               throw std::system_error(
                   errno, std::system_category(),
                   "Could not open '" + fname + "' for reading");
             }
             return self.parse(fp.get());
           })
      .def("print",
           [](const libdap::Error &self) -> std::string {
             std::ostringstream ss;
             self.print(ss);
             return ss.str();
           })
      .def("get_error_code", &libdap::Error::get_error_code)
      .def("get_error_message", &libdap::Error::get_error_message)
      .def("set_error_code", &libdap::Error::set_error_code,
           py::arg("ec") = undefined_error)
      .def("set_error_message", &libdap::Error::set_error_message,
           py::arg("msg") = "");

  py::class_<libdap::InternalErr, libdap::Error>(m, "InternalErr")
      .def(py::init<const std::string &>(), py::arg("msg"))
      .def(py::init<const std::string &, const int, const std::string &>(),
           py::arg("file"), py::arg("line"), py::arg("msg"))
      .def(py::init<>());

  py::register_exception_translator([](std::exception_ptr p) {
    try {
      if (p != nullptr) {
        std::rethrow_exception(p);
      }
    } catch (const libdap::InternalErr &e) {
      auto current_exception = py::cast(
          new libdap::InternalErr(e), py::return_value_policy::take_ownership);
      raise_exception(current_exception);
    } catch (const libdap::Error &e) {
      auto current_exception = py::cast(
          new libdap::Error(e), py::return_value_policy::take_ownership);
      raise_exception(current_exception);
    }
  });
}