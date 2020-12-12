#include "buffer.h"

int Buffer_traverse(Buffer* self, visitproc visit, void* arg) {
    return 0;
}

int Buffer_clear(Buffer* self) {
    free(self->buffer);
    return 0;
}

void Buffer_dealloc(Buffer* self) {
    PyObject_GC_UnTrack(self);
    Buffer_clear(self);
    Py_TYPE(self)->tp_free((PyObject*) self);
}

PyObject* Buffer_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    Buffer* self;
    self = (Buffer*) type->tp_alloc(type, 0);
    if (self != NULL) {
        self->buffer = NULL;
        self->write_pos = 0;
    }
    return (PyObject*) self;
}

int Buffer_init(Buffer* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"size", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|i", kwlist,
                                     &self->size))
        return -1;

    if (self->size == 0) {
        self->size = DEFAULT_BUFFER_SIZE;
    }

    self->buffer = malloc(self->size);

    pthread_cond_init(&self->wait_condition, NULL);
    pthread_mutex_init(&self->wait_mutex, NULL);

    return 0;
}

PyMethodDef Buffer_methods[] = {
    {NULL}  /* Sentinel */
};

PyTypeObject BufferType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pycsdr.Buffer",
    .tp_doc = "Custom objects",
    .tp_basicsize = sizeof(Buffer),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
    .tp_new = Buffer_new,
    .tp_init = (initproc) Buffer_init,
    .tp_dealloc = (destructor) Buffer_dealloc,
    .tp_traverse = (traverseproc) Buffer_traverse,
    .tp_clear = (inquiry) Buffer_clear,
    .tp_methods = Buffer_methods,
};

uint32_t Buffer_getWriteable(Buffer* self) {
    return self->size - self->write_pos;
}

void* Buffer_getWritePointer(Buffer* self) {
    return self->buffer + self->write_pos;
}

void Buffer_advance(Buffer* self, uint32_t how_much) {
    pthread_mutex_lock(&self->wait_mutex);
    self->write_pos = (self->write_pos + how_much) % self->size;
    pthread_cond_broadcast(&self->wait_condition);
    pthread_mutex_unlock(&self->wait_mutex);
}

static uint32_t Buffer_getAvailable(Buffer* self, uint32_t read_pos) {
    if (read_pos <= self->write_pos) {
        return self->write_pos - read_pos;
    } else {
        return self->size - read_pos;
    }
}

uint32_t Buffer_wait(Buffer* self, uint32_t read_pos) {
    uint32_t available;
    pthread_mutex_lock(&self->wait_mutex);
    if ((available = Buffer_getAvailable(self, read_pos)) <= 0) {
        pthread_cond_wait(&self->wait_condition, &self->wait_mutex);
        available = Buffer_getAvailable(self, read_pos);
    }
    pthread_mutex_unlock(&self->wait_mutex);
    return available;
}

void* Buffer_getReadPointer(Buffer* self, uint32_t read_pos) {
    return self->buffer + read_pos;
}