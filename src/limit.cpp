#include "limit.h"
#include <csdr/limit.hpp>
#include <csdr/window.hpp>

static int Limit_init(Limit* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "maxAmplitude", NULL};

    float maxAmplitude = 1.0f;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|f", kwlist, &maxAmplitude)) {
        return -1;
    }

    self->module = new Csdr::Limit(maxAmplitude);

    return 0;
}

static PyMethodDef Limit_methods[] = {
    {"setInput", (PyCFunction) Module_setInput<float, float>, METH_VARARGS | METH_KEYWORDS,
     "set the input buffer"
    },
    {"setOutput", (PyCFunction) Module_setOutput<float, float>, METH_VARARGS | METH_KEYWORDS,
     "set the output buffer"
    },
    {"getOutputFormat", (PyCFunction) Module_getOutputFormat<float>, METH_NOARGS,
     "get output format"
    },
    {"stop", (PyCFunction) Module_stop, METH_NOARGS,
     "stop processing"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot LimitSlots[] = {
    {Py_tp_init, (void*) Limit_init},
    {Py_tp_clear, (void*) Module_clear<float>},
    {Py_tp_methods, Limit_methods},
    {0, 0}
};

PyType_Spec LimitSpec = {
    "pycsdr.modules.Limit",
    sizeof(Limit),
    0,
    Py_TPFLAGS_DEFAULT,
    LimitSlots
};
