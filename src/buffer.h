#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>
#include <pthread.h>

typedef struct {
    PyObject_HEAD
    uint32_t size;
    void* buffer;
    uint32_t write_pos;
    pthread_cond_t wait_condition;
    pthread_mutex_t wait_mutex;
} Buffer;

// 256kb
#define DEFAULT_BUFFER_SIZE 262144

int Buffer_traverse(Buffer* self, visitproc visit, void* arg);
int Buffer_clear(Buffer* self);
void Buffer_dealloc(Buffer* self);
PyObject* Buffer_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
int Buffer_init(Buffer* self, PyObject* args, PyObject* kwds);

uint32_t Buffer_getWriteable(Buffer* self);
void* Buffer_getWritePointer(Buffer* self);
void Buffer_advance(Buffer* self, uint32_t how_much);
uint32_t Buffer_wait(Buffer* self, uint32_t read_pos);
void* Buffer_getReadPointer(Buffer* self, uint32_t read_pos);

extern PyTypeObject BufferType;