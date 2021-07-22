#include "convert.h"
#include "types.h"

#include <csdr/converter.hpp>

static int Convert_init(Convert* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "inFormat", (char*) "outFormat", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!O!", kwlist, FORMAT_TYPE, &self->inFormat, FORMAT_TYPE, &self->outFormat)) {
        return -1;
    }

    // this matrix should be extended in sync with what's available in csdr
    if (self->inFormat == FORMAT_FLOAT) {
        if (self->outFormat == FORMAT_SHORT) {
            self->module = new Csdr::Converter<float, short>();
        } else {
            PyErr_SetString(PyExc_ValueError, "unsupported conversion");
            return -1;
        }
    } else if (self->inFormat == FORMAT_SHORT) {
        if (self->outFormat == FORMAT_FLOAT) {
            self->module = new Csdr::Converter<short, float>();
        } else {
            PyErr_SetString(PyExc_ValueError, "unsupported conversion");
            return -1;
        }
    } else {
        PyErr_SetString(PyExc_ValueError, "unsupported conversion");
        return -1;
    }

    return 0;
}

static PyObject* Convert_setInput(Convert* self, PyObject* args, PyObject* kwds) {
    // this matrix should be extended in sync with what's available in csdr
    if (self->inFormat == FORMAT_FLOAT) {
        if (self->outFormat == FORMAT_SHORT) {
            return Module_setInput<float, short>(self, args, kwds);
        } else {
            PyErr_SetString(PyExc_ValueError, "unsupported conversion");
            return NULL;
        }
    } else if (self->inFormat == FORMAT_SHORT) {
        if (self->outFormat == FORMAT_FLOAT) {
            return Module_setInput<short, float>(self, args, kwds);
        } else {
            PyErr_SetString(PyExc_ValueError, "unsupported conversion");
            return NULL;
        }
    } else {
        PyErr_SetString(PyExc_ValueError, "unsupported conversion");
        return NULL;
    }
}

static PyObject* Convert_setOutput(Convert* self, PyObject* args, PyObject* kwds) {
    // this matrix should be extended in sync with what's available in csdr
    if (self->inFormat == FORMAT_FLOAT) {
        if (self->outFormat == FORMAT_SHORT) {
            return Module_setOutput<float, short>(self, args, kwds);
        } else {
            PyErr_SetString(PyExc_ValueError, "unsupported conversion");
            return NULL;
        }
    } else if (self->inFormat == FORMAT_SHORT) {
        if (self->outFormat == FORMAT_FLOAT) {
            return Module_setOutput<short, float>(self, args, kwds);
        } else {
            PyErr_SetString(PyExc_ValueError, "unsupported conversion");
            return NULL;
        }
    } else {
        PyErr_SetString(PyExc_ValueError, "unsupported conversion");
        return NULL;
    }
}

static PyObject* Convert_getOutputFormat(Convert* self) {
    Py_INCREF(self->outFormat);
    return self->outFormat;
}

static int Convert_clear(Convert* self) {
    if (self->inFormat == FORMAT_FLOAT) {
        return Module_clear<float>(self);
    } else if (self->inFormat == FORMAT_SHORT) {
        return Module_clear<short>(self);
    } else {
        PyErr_SetString(PyExc_ValueError, "unsupported conversion");
        return -1;
    }
}


static PyMethodDef Convert_methods[] = {
    {"setInput", (PyCFunction) Convert_setInput, METH_VARARGS | METH_KEYWORDS,
     "set the input buffer"
    },
    {"setOutput", (PyCFunction) Convert_setOutput, METH_VARARGS | METH_KEYWORDS,
     "set the output buffer"
    },
    {"getOutputFormat", (PyCFunction) Convert_getOutputFormat, METH_NOARGS,
     "get output format"
    },
    {"stop", (PyCFunction) Module_stop, METH_NOARGS,
     "stop processing"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot ConvertSlots[] = {
    {Py_tp_init, (void*) Convert_init},
    {Py_tp_clear, (void*) Convert_clear},
    {Py_tp_methods, Convert_methods},
    {0, 0}
};

PyType_Spec ConvertSpec = {
    "pycsdr.modules.Convert",
    sizeof(Convert),
    0,
    Py_TPFLAGS_DEFAULT,
    ConvertSlots
};

