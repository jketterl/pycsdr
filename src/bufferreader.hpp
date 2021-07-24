#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "reader.hpp"

struct BufferReader: Reader {
    bool run;
};

extern PyType_Spec BufferReaderSpec;