#include "amdemod.h"
#include <csdr/amdemod.hpp>

static int AmDemod_init(AmDemod* self, PyObject* args, PyObject* kwds) {
    self->module = new Csdr::AmDemod();

    return 0;
}

static PyMethodDef AmDemod_methods[] = {
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

static PyType_Slot AmDemodSlots[] = {
    {Py_tp_init, (void*) AmDemod_init},
    {Py_tp_clear, (void*) Module_clear<Csdr::complex<float>>},
    {Py_tp_methods, AmDemod_methods},
    {0, 0}
};

PyType_Spec AmDemodSpec = {
    "pycsdr.modules.AmDemod",
    sizeof(AmDemod),
    0,
    Py_TPFLAGS_DEFAULT,
    AmDemodSlots
};
