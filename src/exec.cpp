#include "exec.hpp"
#include "types.hpp"

#include <csdr/exec.hpp>

#include <iostream>

static int ExecModule_init(ExecModule* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "args", NULL};

    PyObject* args_python = NULL;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist, &PyList_Type, &args_python)) {
        return -1;
    }
    std::vector<std::string> args_vector;
    Py_ssize_t size = PyList_Size(args_python);
    std::cerr << "parsing argument list of length " << size << "\n";

    for (Py_ssize_t i = 0; i < size; i++) {
        PyObject* arg = PyList_GetItem(args_python, i);
        if (!PyUnicode_Check(arg)) {
            PyErr_SetString(PyExc_ValueError, "argument is not a unicode object");
            return -1;
        }
        PyObject* ascii_arg = PyUnicode_AsASCIIString(arg);
        if (ascii_arg == NULL) {
            PyErr_SetString(PyExc_ValueError, "could not handle argument as ascii");
            return -1;
        }
        char* str = PyBytes_AsString(ascii_arg);
        std::cerr << "arg " << i << ": \"" << str << "\"\n";
        args_vector.push_back(std::string(str));
    }

    Py_DECREF(args_python);

    self->inputFormat = FORMAT_SHORT;
    self->outputFormat = FORMAT_SHORT;
    self->setModule(new Csdr::ExecModule<short, short>(args_vector));

    return 0;
}

static PyType_Slot ExecModuleSlots[] = {
    {Py_tp_init, (void*) ExecModule_init},
    {0, 0}
};

PyType_Spec ExecModuleSpec = {
    "pycsdr.modules.ExecModule",
    sizeof(ExecModule),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_FINALIZE,
    ExecModuleSlots
};
