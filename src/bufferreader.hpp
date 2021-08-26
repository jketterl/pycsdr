#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "reader.hpp"
#include "buffer.hpp"

struct BufferReader: Reader {
    Buffer* buffer;
    bool run;
};

extern PyType_Spec BufferReaderSpec;