#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <csdr/module.hpp>
#include <csdr/async.hpp>

template <typename T, typename U>
struct module {
    PyObject base1;
    PyObject base2;
    Csdr::Module<T, U>* module;
    Csdr::AsyncRunner<T, U>* runner = nullptr;
};

template <typename T>
bool isFormatCorrect(PyObject* format);

template <typename T, typename U>
PyObject* Module_setInput(module<T, U>* self, PyObject* args, PyObject* kwds);

template <typename T, typename U>
PyObject* Module_setOutput(module<T, U>* self, PyObject* args, PyObject* kwds);

template <typename T, typename U>
PyObject* Module_stop(module<T, U>* self);