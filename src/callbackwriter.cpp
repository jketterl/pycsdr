#include "callbackwriter.hpp"
#include "types.hpp"

#include <iostream>


template <typename T>
static PyObject* getBytes(T* data, size_t size) {
    PyObject* bytes = PyMemoryView_FromMemory((char*) data, size * sizeof(T), PyBUF_READ);
    return bytes;
}

static int CallbackWriter_init(CallbackWriter* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "format", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist, FORMAT_TYPE, &self->writerFormat)) {
        return -1;
    }

    if (self->writerFormat == FORMAT_CHAR) {
        self->writer = new ProxyWriter<unsigned char>([self] (unsigned char* data, size_t size) {
            // acquire GIL
            PyGILState_STATE gstate;
            gstate = PyGILState_Ensure();

            PyObject* bytes = NULL;
            if (self->writerFormat == FORMAT_CHAR) {
                bytes = getBytes<unsigned char>(data, size);
            }
            if (bytes != NULL) {
                PyObject_CallMethod((PyObject*) self, "write", "O", bytes);
                Py_DECREF(bytes);
            }

            /* Release the thread. No Python API allowed beyond this point. */
            PyGILState_Release(gstate);

        });
    } else {
        Py_DECREF(self->writerFormat);
        PyErr_SetString(PyExc_ValueError, "unsupported writer format");
    }

    return 0;
}

static PyObject* CallbackWriter_write(CallbackWriter* self, PyObject* args, PyObject* kwds) {
    // this is a NOOP, intended to be overridden

    Py_RETURN_NONE;
}

static PyMethodDef CallbackWriter_methods[] = {
    {"write", (PyCFunction) CallbackWriter_write, METH_VARARGS | METH_KEYWORDS,
     "abstract method"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot CallbackWriterSlots[] = {
    {Py_tp_init, (void*) CallbackWriter_init},
    {Py_tp_methods, CallbackWriter_methods},
    {0, 0}
};

PyType_Spec CallbackWriterSpec = {
    "pycsdr.modules.CallbackWriter",
    sizeof(CallbackWriter),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_FINALIZE,
    CallbackWriterSlots
};

template <typename T>
ProxyWriter<T>::ProxyWriter(std::function<void(T* data, size_t size)> callback):
    Csdr::Writer<T>(),
    callback(callback)
{
    buffer = (T*) malloc(sizeof(T) * bufferSize);
}

template <typename T>
ProxyWriter<T>::~ProxyWriter() {
    free(buffer);
}

template <typename T>
size_t ProxyWriter<T>::writeable() {
    return bufferSize;
}

template <typename T>
T* ProxyWriter<T>::getWritePointer() {
    return buffer;
}

template <typename T>
void ProxyWriter<T>::advance(size_t how_much) {
    callback(buffer, how_much);
}