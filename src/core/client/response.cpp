#include <Response.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

static bool file_exists(const std::string& name) {
  struct stat buffer;
  return stat(name.c_str(), &buffer) == 0;
}

static std::fstream py_open(const std::string& name, const char* m) {
  std::ios_base::openmode mode;
  int appending = 0;
  int creating = 0;
  int binary = 0;
  int reading = 0;
  int updating = 0;
  int writing = 0;
  int text = 0;

  switch (*m++) {
    case 'x':
      creating = 1;
      break;
    case 'r':
      reading = 1;
      break;
    case 'w':
      writing = 1;
      break;
    case 'a':
      appending = 1;
      break;
    case '+':
      updating = 1;
      break;
    case 't':
      text = 1;
      break;
    case 'b':
      binary = 1;
      break;
    default:
      throw std::invalid_argument(std::string("invalid mode: '") + *m + "'");
  }

  if (text && binary) {
    throw std::invalid_argument("can't have text and binary mode at once");
  }

  if (creating + reading + writing + appending > 1) {
    throw std::invalid_argument(
        "must have exactly one of create/read/write/append mode");
  }

  if (reading) {
    mode = std::ios_base::in;
  }

  if (writing) {
    mode = std::ios_base::out | std::ios_base::trunc;
  }

  if (appending) {
    mode = std::ios_base::out | std::ios_base::ate;
  }

  if (creating) {
    mode = std::ios_base::out;
    if (file_exists(name)) {
      throw std::system_error(EEXIST, std::system_category(), "'" + name + "'");
    }
  }

  if (binary) {
    mode |= std::ios_base::binary;
  }

  if (updating) {
    mode |= std::ios_base::ate;
  }

  std::fstream result;
  result.exceptions(std::ios::badbit | std::ios::failbit);
  try {
    result.open(name, mode);
  } catch (const std::exception& exc) {
    throw std::system_error(errno, std::system_category(), "'" + name + "'");
  }
  return result;
}

void init_client_response(py::module& m) {
  py::enum_<std::ios_base::seekdir>(m, "SeekDir")
      .value("beg", std::ios_base::beg)
      .value("end", std::ios_base::end)
      .value("cur", std::ios_base::cur)
      .export_values();

  py::class_<std::fstream>(m, "FileStream")
      .def(py::init<>())
      .def(py::init([](const std::string& path, const char* mode) {
             return py_open(path, mode);
           }),
           py::arg("path"), py::arg("mode") = "rb")
      .def("open",
           [](std::fstream& self, const std::string& path, const char* mode) {
             self = py_open(path, mode);
           },
           py::arg("path"), py::arg("mode") = "rb")
      .def("close", &std::fstream::close)
      .def("is_open",
           [](const std::fstream& self) -> bool { return self.is_open(); })
      .def("get", [](std::fstream& self) -> char { return self.get(); })
      .def("get",
           [](std::fstream& self, std::streamsize count) -> py::bytes {
             auto result = std::string(count + 1, 0);
             self.get(&*result.begin(), count);
             return py::bytes(result);
           },
           py::arg("count"))
      .def("get",
           [](std::fstream& self, std::streamsize count,
              char delim) -> py::bytes {
             auto result = std::string(count + 1, 0);
             self.get(&*result.begin(), count, delim);
             return py::bytes(result);
           })
      .def("peek",
           [](std::fstream& self) -> py::bytes {
             char chr = self.peek();
             return py::bytes(&chr);
           })
      .def("unget", [](std::fstream& self) -> void { self.unget(); })
      .def("putback",
           [](std::fstream& self, char chr) -> void { self.putback(chr); },
           py::arg("chr"))
      .def("getline",
           [](std::fstream& self, std::streamsize count) -> py::bytes {
             auto result = std::string(count + 1, 0);
             self.getline(&*result.begin(), count);
             return py::bytes(result);
           },
           py::arg("count"))
      .def("getline",
           [](std::fstream& self, std::streamsize count,
              char delim) -> py::bytes {
             auto result = std::string(count + 1, '\0');
             self.getline(&*result.begin(), count, delim);
             return py::bytes(result.c_str());
           })
      .def("read",
           [](std::fstream& self) -> py::bytes {
             self.seekg(0, std::ios::end);
             size_t size = self.tellg();
             auto result = std::string(size + 1, '\0');
             self.seekg(0);
             self.read(&*result.begin(), size);
             return py::bytes(result.c_str());
           })
      .def("gcount", &std::fstream::gcount)
      .def("tellg",
           [](std::fstream& self) -> std::size_t { return self.tellg(); })
      .def("seekg",
           [](std::fstream& self, std::size_t pos) -> void { self.seekg(pos); },
           py::arg("pos"))
      .def("seekg",
           [](std::fstream& self, std::ptrdiff_t off,
              const std::ios_base::seekdir dir) -> void {
             self.seekg(off, dir);
           },
           py::arg("off"), py::arg("dir"))
      .def("sync", &std::fstream::sync)
      .def("put", [](std::fstream& self, char ch) -> void { self.put(ch); },
           py::arg("ch"))
      .def("write",
           [](std::fstream& self, const py::bytes& bytes) -> void {
             std::string s = bytes;
             self.write(&*s.begin(), s.length() + 1);
           },
           py::arg("bytes"))
      .def("flush", [](std::fstream& self) -> void { self.flush(); })
      .def("good", [](std::fstream& self) -> bool { return self.good(); })
      .def("eof", [](std::fstream& self) -> bool { return self.eof(); })
      .def("fail", [](std::fstream& self) -> bool { return self.fail(); })
      .def("bad", [](std::fstream& self) -> bool { return self.bad(); });

  py::class_<libdap::Response>(m, "Response")
      .def(py::init<>())
      .def(py::init<std::fstream*, int>(), py::arg("stream"),
           py::arg("status") = 0, py::keep_alive<1, 2>())
      .def("get_status", &libdap::Response::get_status)
      .def("get_type", &libdap::Response::get_type)
      .def("get_version", &libdap::Response::get_version)
      .def("get_protocol", &libdap::Response::get_protocol)
      .def("set_status", &libdap::Response::set_status, py::arg("status"))
      .def("set_type", &libdap::Response::set_type, py::arg("type"))
      .def("set_version", &libdap::Response::set_version, py::arg("version"))
      .def("get_cpp_stream",
           [](libdap::Response& self) -> std::fstream* {
             return dynamic_cast<std::fstream*>(self.get_cpp_stream());
           },
           py::return_value_policy::reference_internal)
      .def("set_protocol", &libdap::Response::set_protocol,
           py::arg("protocol"));
}
