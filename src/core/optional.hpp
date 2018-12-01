#pragma once
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <boost/optional.hpp>

namespace pybind11 {
namespace detail {

/// Type_caster for boost::optional
template <typename T>
struct type_caster<boost::optional<T>>
    : public optional_caster<boost::optional<T>> {};

}  // namespace detail
}  // namespace pybind11
