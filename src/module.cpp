#include "module.h"
#include "types.h"
#include "buffer.h"
#include <csdr/ringbuffer.hpp>

template<>
PyObject* getFormat<unsigned char>() {
    return FORMAT_CHAR;
}

template <>
PyObject* getFormat<short>() {
    return FORMAT_SHORT;
}

template <>
PyObject* getFormat<float>() {
    return FORMAT_FLOAT;
}

template<>
PyObject* getFormat<Csdr::complex<float>>() {
    return FORMAT_COMPLEX_FLOAT;
}

template <typename T, typename U>
PyObject* Module_getOutputFormat(module<T, U>* self) {
    return getFormat<U>();
}

template PyObject* Module_getOutputFormat(module<Csdr::complex<float>, Csdr::complex<float>>* self);
template PyObject* Module_getOutputFormat(module<Csdr::complex<float>, float>* self);
template PyObject* Module_getOutputFormat(module<float, float>* self);
template PyObject* Module_getOutputFormat(module<float, unsigned char>* self);

template <typename T, typename U>
PyObject* Module_setInput(module<T, U>* self, PyObject* args, PyObject* kwds) {
    Buffer* buffer;

    static char* kwlist[] = {(char*) "reader", NULL};
    // TODO get the type check "O!" back
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &buffer)) {
        return NULL;
    }

    if (getFormat<T>() != buffer->format) {
        PyErr_SetString(PyExc_ValueError, "invalid reader format");
        return NULL;
    }

    if (self->input != nullptr) {
        Py_DECREF(self->input);
    }
    self->input = buffer;
    Py_INCREF(self->input);

    Csdr::Ringbuffer<T>* b = dynamic_cast<Csdr::Ringbuffer<T>*>(buffer->writer);
    Csdr::Reader<T>* reader = new Csdr::RingbufferReader<T>(b);
    self->module->setReader(reader);

    if (self->module->hasReader() && self->module->hasWriter() && self->runner == nullptr) {
        self->runner = new Csdr::AsyncRunner<T, U>(self->module);
    }

    Py_RETURN_NONE;
}

template PyObject* Module_setInput(module<Csdr::complex<float>, Csdr::complex<float>>* self, PyObject* args, PyObject* kwds);
template PyObject* Module_setInput(module<Csdr::complex<float>, float>* self, PyObject* args, PyObject* kwds);
template PyObject* Module_setInput(module<float, float>* self, PyObject* args, PyObject* kwds);
template PyObject* Module_setInput(module<float, unsigned char>* self, PyObject* args, PyObject* kwds);

template <typename T, typename U>
PyObject* Module_setOutput(module<T, U>* self, PyObject* args, PyObject* kwds) {
    Buffer* buffer;

    static char* kwlist[] = {(char*) "writer", NULL};
    // TODO get the type check "O!" back
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &buffer)) {
        return NULL;
    }

    if (getFormat<U>() != buffer->format) {
        PyErr_SetString(PyExc_ValueError, "invalid writer format");
        return NULL;
    }

    if (self->output != nullptr) {
        Py_DECREF(self->output);
    }
    self->output = buffer;
    Py_INCREF(self->output);

    self->module->setWriter(dynamic_cast<Csdr::Writer<U>*>(buffer->writer));

    if (self->module->hasReader() && self->module->hasWriter() && self->runner == nullptr) {
        self->runner = new Csdr::AsyncRunner<T, U>(self->module);
    }

    Py_RETURN_NONE;
}

template PyObject* Module_setOutput(module<Csdr::complex<float>, Csdr::complex<float>>* self, PyObject* args, PyObject* kwds);
template PyObject* Module_setOutput(module<Csdr::complex<float>, float>* self, PyObject* args, PyObject* kwds);
template PyObject* Module_setOutput(module<float, float>* self, PyObject* args, PyObject* kwds);
template PyObject* Module_setOutput(module<float, unsigned char>* self, PyObject* args, PyObject* kwds);

template <typename T, typename U>
PyObject* Module_stop(module<T, U>* self) {
    if (self->runner != nullptr) {
        self->runner->stop();
        delete self->runner;
    }
    Py_RETURN_NONE;
}

template PyObject* Module_stop(module<Csdr::complex<float>, Csdr::complex<float>>* self);
template PyObject* Module_stop(module<Csdr::complex<float>, float>* self);
template PyObject* Module_stop(module<float, float>* self);
template PyObject* Module_stop(module<float, unsigned char>* self);

template <typename T, typename U>
int Module_clear(module<T, U>* self) {
    Module_stop(self);

    if (self->output != nullptr) {
        Py_DECREF(self->output);
        self->output = nullptr;
    }

    if (self->input != nullptr) {
        Py_DECREF(self->input);
        self->input = nullptr;
    }

    delete self->module;

    return 0;
}

template int Module_clear(module<Csdr::complex<float>, Csdr::complex<float>>* self);
template int Module_clear(module<Csdr::complex<float>, float>* self);
template int Module_clear(module<float, float>* self);
template int Module_clear(module<float, unsigned char>* self);
