#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>

typedef struct {
    PyObject_HEAD
    uint32_t size;
    void* buffer;
} Buffer;

#define DEFAULT_BUFFER_SIZE 65535


int Buffer_traverse(Buffer* self, visitproc visit, void* arg);
int Buffer_clear(Buffer* self);
void Buffer_dealloc(Buffer* self);
PyObject* Buffer_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
int Buffer_init(Buffer* self, PyObject* args, PyObject* kwds);

extern PyTypeObject BufferType;