#include "fmdemod.h"
#include <csdr/fmdemod.hpp>

static int FmDemod_init(FmDemod* self, PyObject* args, PyObject* kwds) {
    self->module = new Csdr::FmDemod();

    return 0;
}

static PyMethodDef FmDemod_methods[] = {
    {"setInput", (PyCFunction) Module_setInput<Csdr::complex<float>, float>, METH_VARARGS | METH_KEYWORDS,
     "set the input buffer"
    },
    {"setOutput", (PyCFunction) Module_setOutput<Csdr::complex<float>, float>, METH_VARARGS | METH_KEYWORDS,
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

static PyType_Slot FmDemodSlots[] = {
    {Py_tp_init, (void*) FmDemod_init},
    {Py_tp_clear, (void*) Module_clear<Csdr::complex<float>>},
    {Py_tp_methods, FmDemod_methods},
    {0, 0}
};

PyType_Spec FmDemodSpec = {
    "pycsdr.modules.FmDemod",
    sizeof(FmDemod),
    0,
    Py_TPFLAGS_DEFAULT,
    FmDemodSlots
};
