#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "module.hpp"

struct Agc: Module {};

extern PyType_Spec AgcSpec;