#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

extern PyTypeObject* WriterType;

extern PyTypeObject* ReaderType;

extern PyTypeObject* SinkType;

extern PyTypeObject* SourceType;

extern PyTypeObject* ModuleType;

extern PyTypeObject* BufferType;

extern PyTypeObject* BufferReaderType;