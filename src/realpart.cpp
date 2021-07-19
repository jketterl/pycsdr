#include "realpart.h"
#include <csdr/realpart.hpp>

static int RealPart_init(RealPart* self, PyObject* args, PyObject* kwds) {
    self->module = new Csdr::Realpart();

    return 0;
}

static PyMethodDef RealPart_methods[] = {
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

static PyType_Slot RealPartSlots[] = {
    {Py_tp_init, (void*) RealPart_init},
    {Py_tp_clear, (void*) Module_clear<Csdr::complex<float>>},
    {Py_tp_methods, RealPart_methods},
    {0, 0}
};

PyType_Spec RealPartSpec = {
    "pycsdr.modules.RealPart",
    sizeof(RealPart),
    0,
    Py_TPFLAGS_DEFAULT,
    RealPartSlots
};
