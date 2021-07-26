#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "writer.hpp"

struct Buffer: Writer {};

// 256kb
#define DEFAULT_BUFFER_SIZE 262144

extern PyType_Spec BufferSpec;