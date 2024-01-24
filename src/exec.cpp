#include "exec.hpp"
#include "types.hpp"

#include <csdr/exec.hpp>

static int convertArgs(PyObject* args_python, std::vector<std::string>& args_vector) {
    Py_ssize_t size = PyList_Size(args_python);

    for (Py_ssize_t i = 0; i < size; i++) {
        PyObject* arg = PyList_GetItem(args_python, i);
        if (arg == NULL) {
            // error is already set by PyList_GetItem()
            return -1;
        }
        if (!PyUnicode_Check(arg)) {
            PyErr_SetString(PyExc_ValueError, "argument is not a unicode object");
            return -1;
        }
        Py_ssize_t str_len;
        const char* str = PyUnicode_AsUTF8AndSize(arg, &str_len);
        if (str == NULL) {
            // error is already set by PyUnicode_AsUTF8AndSize()
            return -1;
        }
        args_vector.push_back(std::string(str, str_len));
    }
    return 0;
}

static int ExecModule_init(ExecModule* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "args", (char*) "inFormat", (char*) "outFormat", (char*) "flushSize", NULL};

    PyObject* inFormat;
    PyObject* outFormat;
    PyObject* args_python = NULL;
    unsigned int flushSize = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!O!O!|I", kwlist, FORMAT_TYPE, &inFormat, FORMAT_TYPE, &outFormat, &PyList_Type, &args_python, &flushSize)) {
        return -1;
    }
    std::vector<std::string> args_vector;
    if (convertArgs(args_python, args_vector) != 0) {
        return -1;
    }

    if (inFormat == FORMAT_SHORT) {
        if (outFormat == FORMAT_SHORT) {
            self->setModule(new Csdr::ExecModule<short, short>(args_vector, flushSize));
        } else if (outFormat == FORMAT_CHAR) {
            self->setModule(new Csdr::ExecModule<short, unsigned char>(args_vector, flushSize));
        } else {
            PyErr_SetString(PyExc_ValueError, "invalid output format");
            return -1;
        }
    } else if (inFormat == FORMAT_COMPLEX_SHORT) {
        if (outFormat == FORMAT_SHORT) {
            self->setModule(new Csdr::ExecModule<Csdr::complex<short>, short>(args_vector, flushSize));
        } else if (outFormat == FORMAT_CHAR) {
            self->setModule(new Csdr::ExecModule<Csdr::complex<short>, unsigned char>(args_vector, flushSize));
        } else {
            PyErr_SetString(PyExc_ValueError, "invalid output format");
            return -1;
        }
    } else if (inFormat == FORMAT_CHAR) {
        if (outFormat == FORMAT_CHAR) {
            self->setModule(new Csdr::ExecModule<unsigned char, unsigned char>(args_vector, flushSize));
        } else if (outFormat == FORMAT_FLOAT) {
            self->setModule(new Csdr::ExecModule<unsigned char, float>(args_vector, flushSize));
        } else {
            PyErr_SetString(PyExc_ValueError, "invalid output format");
            return -1;
        }
    } else if (inFormat == FORMAT_COMPLEX_FLOAT) {
        if (outFormat == FORMAT_CHAR) {
            self->setModule(new Csdr::ExecModule<Csdr::complex<float>, unsigned char>(args_vector, flushSize));
        } else {
            PyErr_SetString(PyExc_ValueError, "invalid output format");
            return -1;
        }
    } else if (inFormat == FORMAT_COMPLEX_CHAR) {
        if (outFormat == FORMAT_SHORT) {
            self->setModule(new Csdr::ExecModule<Csdr::complex<unsigned char>, short>(args_vector, flushSize));
        } else {
            PyErr_SetString(PyExc_ValueError, "invalid output format");
            return -1;
        }
    } else {
        PyErr_SetString(PyExc_ValueError, "invalid input format");
        return -1;
    }

    Py_INCREF(inFormat);
    self->inputFormat = inFormat;
    Py_INCREF(outFormat);
    self->outputFormat = outFormat;

    return 0;
}

static PyObject* ExecModule_reload(ExecModule* self) {
    dynamic_cast<Csdr::UntypedExecModule*>(self->module)->reload();
    Py_RETURN_NONE;
}

static PyObject* ExecModule_restart(ExecModule* self) {
    dynamic_cast<Csdr::UntypedExecModule*>(self->module)->restart();
    Py_RETURN_NONE;
}

static PyObject* ExecModule_setArgs(ExecModule* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "args", NULL};
    PyObject* args_python = NULL;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist, &PyList_Type, &args_python)) {
        return NULL;
    }
    std::vector<std::string> args_vector;
    if (convertArgs(args_python, args_vector) != 0) {
        return NULL;
    }
    dynamic_cast<Csdr::UntypedExecModule*>(self->module)->setArgs(args_vector);
    Py_RETURN_NONE;
}

static PyMethodDef ExecModule_methods[] = {
    {"reload", (PyCFunction) ExecModule_reload, METH_NOARGS,
     "sends a SIGHUP to the child process. check if this feature is available on the program in use."
    },
    {"restart", (PyCFunction) ExecModule_restart, METH_NOARGS,
     "if SIGHUP is not available, you can perform a hard restart."
    },
    {"setArgs", (PyCFunction) ExecModule_setArgs, METH_VARARGS | METH_KEYWORDS,
     "set a new argument list"
    },
    {NULL}  /* Sentinel */
};


static PyType_Slot ExecModuleSlots[] = {
    {Py_tp_init, (void*) ExecModule_init},
    {Py_tp_methods, ExecModule_methods},
    {0, 0}
};

PyType_Spec ExecModuleSpec = {
    "pycsdr.modules.ExecModule",
    sizeof(ExecModule),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_FINALIZE,
    ExecModuleSlots
};
