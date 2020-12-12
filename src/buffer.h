#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>

typedef struct {
    PyObject_HEAD
    uint32_t size;
    void* buffer;
    uint32_t write_pos;
} Buffer;

#define DEFAULT_BUFFER_SIZE 65535


int Buffer_traverse(Buffer* self, visitproc visit, void* arg);
int Buffer_clear(Buffer* self);
void Buffer_dealloc(Buffer* self);
PyObject* Buffer_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
int Buffer_init(Buffer* self, PyObject* args, PyObject* kwds);

uint32_t Buffer_getAvailable(Buffer* self);
void* Buffer_getWritePointer(Buffer* self);
void Buffer_advance(Buffer* self, uint32_t how_much);

extern PyTypeObject BufferType;