#include "buffer.h"

static int Buffer_clear(Buffer* self) {
    if (self->buffer != NULL) free(self->buffer);
    self->buffer = NULL;
    pthread_cond_destroy(&self->wait_condition);
    pthread_mutex_destroy(&self->wait_mutex);
    return 0;
}

static int Buffer_init(Buffer* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"size", "item_size", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|IB", kwlist,
                                     &self->size, &self->item_size))
        return -1;

    if (self->size == 0) {
        self->size = DEFAULT_BUFFER_SIZE;
    }
    self->end_pos = self->size;

    self->buffer = malloc(self->size * self->item_size);

    pthread_cond_init(&self->wait_condition, NULL);
    pthread_mutex_init(&self->wait_mutex, NULL);

    self->run = true;

    return 0;
}

static PyObject* Buffer_read(Buffer* self, PyObject* Py_UNUSED(ignored)) {
    uint32_t available;
    Py_BEGIN_ALLOW_THREADS
    available = Buffer_wait(self, self->read_pos, &self->run);
    Py_END_ALLOW_THREADS
    if (!self->run && available == 0) {
        Py_RETURN_NONE;
    } else {
        PyObject* bytes = PyMemoryView_FromMemory(Buffer_getReadPointer(self, self->read_pos), available * self->item_size, PyBUF_READ);
        Buffer_advanceReadPos(self, &self->read_pos, available);
        return bytes;
    }
}

static PyObject* Buffer_stop(Buffer* self, PyObject* Py_UNUSED(ignored)) {
    self->run = false;
    Buffer_unblock(self);
    Py_RETURN_NONE;
}

static PyMethodDef Buffer_methods[] = {
    {"read", (PyCFunction) Buffer_read, METH_NOARGS,
     "read bytes from the buffer"},
    {"stop", (PyCFunction) Buffer_stop, METH_NOARGS,
     "stop the buffer and unblock calls to read()"},
    {NULL}  /* Sentinel */
};

static PyType_Slot BufferSlots[] = {
    {Py_tp_init, Buffer_init},
    {Py_tp_clear, Buffer_clear},
    {Py_tp_methods, Buffer_methods},
    {0, 0}
};

PyType_Spec BufferSpec = {
    "pycsdr.modules.Buffer",
    sizeof(Buffer),
    0,
    Py_TPFLAGS_DEFAULT,
    BufferSlots
};

void Buffer_setItemSize(Buffer* self, uint8_t item_size) {
    if (self->item_size == item_size) {
        return;
    }
    self->item_size = item_size;

    void* waste = self->buffer;
    self->buffer = malloc(self->size * self->item_size);
    free(waste);
}

uint8_t Buffer_getItemSize(Buffer* self) {
    return self->item_size;
}

uint32_t Buffer_getWriteable(Buffer* self) {
    return self->size - self->write_pos;
}

void* Buffer_getWritePointer(Buffer* self) {
    return self->buffer + self->write_pos * self->item_size;
}

void* Buffer_getWritePointer_n(Buffer* self, uint32_t n) {
    if (Buffer_getWriteable(self) < n) {
        self->end_pos = self->write_pos;
        self->write_pos = 0;
    }
    return Buffer_getWritePointer(self);
}

void Buffer_advance(Buffer* self, uint32_t how_much) {
    pthread_mutex_lock(&self->wait_mutex);
    self->write_pos = (self->write_pos + how_much) % self->size;
    if (self->write_pos > self->end_pos) {
        self->end_pos = self->write_pos;
    } else if (self->write_pos == 0) {
        self->end_pos = self->size;
    }
    pthread_cond_broadcast(&self->wait_condition);
    pthread_mutex_unlock(&self->wait_mutex);
}

void Buffer_advanceReadPos(Buffer* self, uint32_t* read_pos, uint32_t how_much) {
    if (*read_pos >= self->end_pos) {
        *read_pos = how_much;
    } else {
        *read_pos = (*read_pos + how_much) % self->size;
    }
}

static uint32_t Buffer_getAvailable(Buffer* self, uint32_t read_pos) {
    if (read_pos <= self->write_pos) {
        return self->write_pos - read_pos;
    } else if (read_pos >= self->end_pos) {
        return self->write_pos;
    } else {
        return self->end_pos - read_pos;
    }
}

uint32_t Buffer_wait(Buffer* self, uint32_t read_pos, bool* run) {
    return Buffer_wait_n(self, read_pos, run, 1);
}

uint32_t Buffer_wait_n(Buffer* self, uint32_t read_pos, bool* run, uint32_t n) {
    uint32_t available = 0;
    pthread_mutex_lock(&self->wait_mutex);
    while (*run && (available = Buffer_getAvailable(self, read_pos)) < n) {
        pthread_cond_wait(&self->wait_condition, &self->wait_mutex);
    }
    pthread_mutex_unlock(&self->wait_mutex);
    return available;
}

void* Buffer_getReadPointer(Buffer* self, uint32_t read_pos) {
    if (read_pos >= self->end_pos) {
        return self->buffer;
    }
    return self->buffer + read_pos * self->item_size;
}

uint32_t Buffer_read_n(Buffer* self, void* dst, uint32_t* read_pos, bool* run, uint32_t n) {
    uint32_t available;
    uint32_t read = 0;
    while (*run && read < n) {
        available = Buffer_wait(self, *read_pos, run);
        if (!*run) break;

        if (available > n - read) available = n - read;
        memcpy(dst + read * self->item_size, Buffer_getReadPointer(self, *read_pos), available * self->item_size);
        read += available;
        *read_pos = (*read_pos + available) % self->size;
    }
    return read;
}

uint32_t Buffer_skip_n(Buffer* self, uint32_t* read_pos, bool* run, uint32_t n) {
    uint32_t available;
    uint32_t read = 0;
    while (*run && read < n) {
        available = Buffer_wait(self, *read_pos, run);
        if (!*run) break;

        if (available > n - read) available = n - read;
        read += available;
        *read_pos = (*read_pos + available) % self->size;
    }
    return read;
}

void Buffer_write(Buffer* self, void* src, uint32_t len) {
    uint32_t remaining = len;
    uint32_t writeable;
    while (self->run && remaining > 0) {
        writeable = Buffer_getWriteable(self);
        if (writeable > remaining) writeable = remaining;
        memcpy(Buffer_getWritePointer(self), src + ((len - remaining) * self->item_size), writeable * self->item_size);
        Buffer_advance(self, writeable);
        remaining -= writeable;
    }
}

void Buffer_unblock(Buffer* self) {
    pthread_mutex_lock(&self->wait_mutex);
    pthread_cond_broadcast(&self->wait_condition);
    pthread_mutex_unlock(&self->wait_mutex);
}