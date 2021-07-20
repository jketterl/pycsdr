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

template <typename U>
PyObject* Module_getOutputFormat(Module* self) {
    return getFormat<U>();
}

template PyObject* Module_getOutputFormat<Csdr::complex<float>>(Module* self);
template PyObject* Module_getOutputFormat<float>(Module* self);
template PyObject* Module_getOutputFormat<unsigned char>(Module* self);
template PyObject* Module_getOutputFormat<short>(Module* self);

template <typename T, typename U>
PyObject* Module_setInput(Module* self, PyObject* args, PyObject* kwds) {
    Buffer* buffer;

    static char* kwlist[] = {(char*) "reader", NULL};
    // TODO get the type check "O!" back
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &buffer)) {
        return NULL;
    }

    if ((PyObject*) buffer != Py_None && getFormat<T>() != buffer->format) {
        PyErr_SetString(PyExc_ValueError, "invalid reader format");
        return NULL;
    }

    auto module = dynamic_cast<Csdr::Module<T, U>*>(self->module);

    auto oldReader = module->getReader();

    if (self->input != nullptr) {
        Py_DECREF(self->input);
        self->input = nullptr;
    }

    if ((PyObject*) buffer != Py_None) {
        self->input = buffer;
        Py_INCREF(self->input);

        Csdr::Ringbuffer <T> *b = dynamic_cast<Csdr::Ringbuffer <T> *>(buffer->writer);
        Csdr::Reader <T> *reader = new Csdr::RingbufferReader<T>(b);
        module->setReader(reader);

        if (module->hasWriter() && self->runner == nullptr) {
            self->runner = new Csdr::AsyncRunner<T, U>(module);
        }
    } else {
        module->setReader(nullptr);
    }

    // we created it, we discard of it
    delete oldReader;

    Py_RETURN_NONE;
}

template PyObject* Module_setInput<Csdr::complex<float>, Csdr::complex<float>>(Module* self, PyObject* args, PyObject* kwds);
template PyObject* Module_setInput<Csdr::complex<float>, float>(Module* self, PyObject* args, PyObject* kwds);
template PyObject* Module_setInput<float, float>(Module* self, PyObject* args, PyObject* kwds);
template PyObject* Module_setInput<float, unsigned char>(Module* self, PyObject* args, PyObject* kwds);
template PyObject* Module_setInput<float, short>(Module* self, PyObject* args, PyObject* kwds);
template PyObject* Module_setInput<short, float>(Module* self, PyObject* args, PyObject* kwds);
template PyObject* Module_setInput<short, short>(Module* self, PyObject* args, PyObject* kwds);

template <typename T, typename U>
PyObject* Module_setOutput(Module* self, PyObject* args, PyObject* kwds) {
    Buffer* buffer;

    static char* kwlist[] = {(char*) "writer", NULL};
    // TODO get the type check "O!" back
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &buffer)) {
        return NULL;
    }

    if ((PyObject*) buffer != Py_None && getFormat<U>() != buffer->format) {
        PyErr_SetString(PyExc_ValueError, "invalid writer format");
        return NULL;
    }

    if (self->output != nullptr) {
        Py_DECREF(self->output);
        self->output = nullptr;
    }

    auto module = dynamic_cast<Csdr::Module<T, U>*>(self->module);

    if ((PyObject*) buffer != Py_None) {
        self->output = buffer;
        Py_INCREF(self->output);

        module->setWriter(dynamic_cast<Csdr::Writer <U> *>(buffer->writer));

        if (module->hasReader() && self->runner == nullptr) {
            self->runner = new Csdr::AsyncRunner<T, U>(module);
        }
    } else {
        module->setWriter(nullptr);
    }

    Py_RETURN_NONE;
}

template PyObject* Module_setOutput<Csdr::complex<float>, Csdr::complex<float>>(Module* self, PyObject* args, PyObject* kwds);
template PyObject* Module_setOutput<Csdr::complex<float>, float>(Module* self, PyObject* args, PyObject* kwds);
template PyObject* Module_setOutput<float, float>(Module* self, PyObject* args, PyObject* kwds);
template PyObject* Module_setOutput<float, unsigned char>(Module* self, PyObject* args, PyObject* kwds);
template PyObject* Module_setOutput<float, short>(Module* self, PyObject* args, PyObject* kwds);
template PyObject* Module_setOutput<short, float>(Module* self, PyObject* args, PyObject* kwds);
template PyObject* Module_setOutput<short, short>(Module* self, PyObject* args, PyObject* kwds);

PyObject* Module_stop(Module* self) {
    if (self->runner != nullptr) {
        self->runner->stop();
        delete self->runner;
        self->runner = nullptr;
    }
    Py_RETURN_NONE;
}

template <typename T>
int Module_clear(Module* self) {
    Module_stop(self);

    if (self->output != nullptr) {
        Py_DECREF(self->output);
        self->output = nullptr;
    }

    if (self->input != nullptr) {
        auto module = dynamic_cast<Csdr::Sink<T>*>(self->module);
        auto reader = module->getReader();
        module->setReader(nullptr);
        delete reader;
        Py_DECREF(self->input);
        self->input = nullptr;
    }

    delete self->module;

    return 0;
}

template int Module_clear<Csdr::complex<float>>(Module* self);
template int Module_clear<float>(Module* self);
template int Module_clear<short>(Module* self);
