#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <csdr/reader.hpp>

struct Reader {
    PyObject_HEAD
    Csdr::UntypedReader* reader = nullptr;
    PyObject* readerFormat;
};

int Reader_finalize(Reader* self);

extern PyType_Spec ReaderSpec;