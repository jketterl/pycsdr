#include "module.h"
#include "types.h"
#include "buffer.h"

template<>
bool isFormatCorrect<unsigned char>(PyObject* format) {
    return format == FORMAT_CHAR;
}

template<>
bool isFormatCorrect<short>(PyObject* format) {
    return format == FORMAT_SHORT;
}

template<>
bool isFormatCorrect<float>(PyObject* format) {
    return format == FORMAT_FLOAT;
}

template<>
bool isFormatCorrect<Csdr::complex<float>>(PyObject* format) {
    return format == FORMAT_COMPLEX_FLOAT;
}

template <typename T, typename U>
PyObject* Module_setInput(module<T, U>* self, PyObject* args, PyObject* kwds) {
    Buffer* buffer;

    static char* kwlist[] = {(char*) "reader", NULL};
    // TODO get the type check "O!" back
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &buffer))
        return NULL;

    // we only support complex float here.
    if (isFormatCorrect<T>(buffer->format)) {
        self->module->setReader(dynamic_cast<Csdr::Reader<T>*>(buffer->reader));
    } else {
        PyErr_SetString(PyExc_ValueError, "invalid reader format");
        return NULL;
    }

    if (self->module->hasReader() && self->module->hasWriter()) {
        self->runner = new Csdr::AsyncRunner<T, U>(self->module);
    }

    Py_RETURN_NONE;
}

template PyObject* Module_setInput(module<Csdr::complex<float>, Csdr::complex<float>>* self, PyObject* args, PyObject* kwds);
template PyObject* Module_setInput(module<Csdr::complex<float>, float>* self, PyObject* args, PyObject* kwds);

template <typename T, typename U>
PyObject* Module_setOutput(module<T, U>* self, PyObject* args, PyObject* kwds) {
    Buffer* buffer;

    static char* kwlist[] = {(char*) "writer", NULL};
    // TODO get the type check "O!" back
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &buffer))
        return NULL;

    // we only support complex float here.
    if (isFormatCorrect<U>(buffer->format)) {
        self->module->setWriter(dynamic_cast<Csdr::Writer<U>*>(buffer->writer));
    } else {
        PyErr_SetString(PyExc_ValueError, "invalid format");
        return NULL;
    }

    if (self->module->hasReader() && self->module->hasWriter()) {
        self->runner = new Csdr::AsyncRunner<T, U>(self->module);
    }

    Py_RETURN_NONE;
}

template PyObject* Module_setOutput(module<Csdr::complex<float>, Csdr::complex<float>>* self, PyObject* args, PyObject* kwds);
template PyObject* Module_setOutput(module<Csdr::complex<float>, float>* self, PyObject* args, PyObject* kwds);

template <typename T, typename U>
PyObject* Module_stop(module<T, U>* self) {
    self->runner->stop();
    Py_RETURN_NONE;
}

template PyObject* Module_stop(module<Csdr::complex<float>, Csdr::complex<float>>* self);
template PyObject* Module_stop(module<Csdr::complex<float>, float>* self);