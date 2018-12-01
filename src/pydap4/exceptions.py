from . import core


REGISTRY = {}


def register(cls):
    """A Python decorator that adds a Python exception wrapper to the
    registry that maps C++ Exceptions to their Python wrapper classes.
    """
    REGISTRY[cls.WrappedClass] = cls
    return cls


class ExceptionMeta(type):
    """A metaclass for custom exception wrappers, which adds lookup of class
    attributes by delegating to the C++ wrapped class.
    """
    def __getattr__(cls, name):
        return getattr(cls.WrappedClass, name)


@register
class Error(Exception, metaclass=ExceptionMeta):
    """The base class for Python-wrapped C++ exceptions.
    """
    WrappedClass = core.Error

    def __init__(self, arg, *args, **kwds):
        if isinstance(arg, core.Error):
            cpp = arg
            message = cpp.get_error_message()
        else:
            message = arg
            cpp = self.WrappedClass(message, *args, **kwds)
        super(Exception, self).__init__(message)
        self.cpp = cpp

    def __getattr__(self, name):
        return getattr(self.cpp, name)

    def __repr__(self):
        return "%s(%r)" % (type(self).__name__, self.cpp.get_error_message())

    def __str__(self):
        return self.cpp.print()


@register
class InternalErr(Error):
    WrappedClass = core.InternalErr


def translate(cpp):
    """Translate a C++ Exception instance to Python and return it."""
    PyType = REGISTRY.get(type(cpp), None)
    if PyType is None:
        raise RuntimeError(
            "Could not find appropriate Python type for C++ Exception")
        PyType = Exception
    return PyType(cpp)
