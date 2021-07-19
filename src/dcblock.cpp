#include "dcblock.h"
#include <csdr/dcblock.hpp>

static int DcBlock_init(DcBlock* self, PyObject* args, PyObject* kwds) {
    self->module = new Csdr::DcBlock();

    return 0;
}

static PyMethodDef DcBlock_methods[] = {
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

static PyType_Slot DcBlockSlots[] = {
    {Py_tp_init, (void*) DcBlock_init},
    {Py_tp_clear, (void*) Module_clear<float>},
    {Py_tp_methods, DcBlock_methods},
    {0, 0}
};

PyType_Spec DcBlockSpec = {
    "pycsdr.modules.DcBlock",
    sizeof(DcBlock),
    0,
    Py_TPFLAGS_DEFAULT,
    DcBlockSlots
};
