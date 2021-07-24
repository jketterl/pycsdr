#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <csdr/source.hpp>
#include <csdr/complex.hpp>

#include "source.hpp"

struct TcpSource: Source {};

extern PyType_Spec TcpSourceSpec;