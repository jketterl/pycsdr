#include "logpower.h"
#include <csdr/logpower.hpp>

static int LogPower_init(LogPower* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "add_db", NULL};

    float add_db = 0.0f;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "f", kwlist, &add_db)) {
        return -1;
    }

    self->module = new Csdr::LogPower(add_db);

    return 0;
}

static PyMethodDef LogPower_methods[] = {
    {"setInput", (PyCFunction) Module_setInput<Csdr::complex<float>, float>, METH_VARARGS | METH_KEYWORDS,
     "set the input buffer"
    },
    {"setOutput", (PyCFunction) Module_setOutput<Csdr::complex<float>, float>, METH_VARARGS | METH_KEYWORDS,
     "set the output buffer"
    },
    {"getOutputFormat", (PyCFunction) Module_getOutputFormat<Csdr::complex<float>, float>, METH_NOARGS,
     "get output format"
    },
    {"stop", (PyCFunction) Module_stop<Csdr::complex<float>, float>, METH_NOARGS,
     "stop processing"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot LogPowerSlots[] = {
    {Py_tp_init, (void*) LogPower_init},
    {Py_tp_clear, (void*) Module_clear<Csdr::complex<float>, float>},
    {Py_tp_methods, LogPower_methods},
    {0, 0}
};

PyType_Spec LogPowerSpec = {
    "pycsdr.modules.LogPower",
    sizeof(LogPower),
    0,
    Py_TPFLAGS_DEFAULT,
    LogPowerSlots
};
