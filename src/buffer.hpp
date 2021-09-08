#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "writer.hpp"

struct Buffer: Writer {
    size_t writeOverhang;
};

// 256kb
#define DEFAULT_BUFFER_SIZE 262144

extern PyType_Spec BufferSpec;