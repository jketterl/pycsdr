#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct {
    PyObject_HEAD
    uint32_t size;
    void* buffer;
    uint8_t item_size;
    uint32_t write_pos;
    uint32_t read_pos;
    pthread_cond_t wait_condition;
    pthread_mutex_t wait_mutex;
    bool run;
} Buffer;

// 256kb
#define DEFAULT_BUFFER_SIZE 262144

int Buffer_traverse(Buffer* self, visitproc visit, void* arg);
int Buffer_clear(Buffer* self);
void Buffer_dealloc(Buffer* self);
PyObject* Buffer_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
int Buffer_init(Buffer* self, PyObject* args, PyObject* kwds);
PyObject* Buffer_read(Buffer* self, PyObject* Py_UNUSED(ignored));

uint8_t Buffer_getItemSize(Buffer* self);
uint32_t Buffer_getWriteable(Buffer* self);
void* Buffer_getWritePointer(Buffer* self);
void Buffer_advance(Buffer* self, uint32_t how_much);
void Buffer_advanceReadPos(Buffer* self, uint32_t* read_pos, uint32_t how_much);
uint32_t Buffer_wait(Buffer* self, uint32_t read_pos);
uint32_t Buffer_wait_n(Buffer* self, uint32_t read_pos, uint32_t n);
void* Buffer_getReadPointer(Buffer* self, uint32_t read_pos);
uint32_t Buffer_read_n(Buffer* self, void* dst, uint32_t* read_pos, uint32_t n);
uint32_t Buffer_skip_n(Buffer* self, uint32_t* read_pos, uint32_t n);
void Buffer_write(Buffer* self, void* src, uint32_t len);
void Buffer_shutdown(Buffer* self);

extern PyTypeObject BufferType;