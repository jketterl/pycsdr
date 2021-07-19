#include "nfmdeemphasis.h"
#include <csdr/deemphasis.hpp>

static int NfmDeemphasis_init(NfmDeemphasis* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "sampleRate", NULL};

    unsigned int sampleRate = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "I", kwlist, &sampleRate)) {
        return -1;
    }

    self->module = new Csdr::NfmDeephasis(sampleRate);

    return 0;
}

static PyMethodDef NfmDeemphasis_methods[] = {
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

static PyType_Slot NfmDeemphasisSlots[] = {
    {Py_tp_init, (void*) NfmDeemphasis_init},
    {Py_tp_clear, (void*) Module_clear<float>},
    {Py_tp_methods, NfmDeemphasis_methods},
    {0, 0}
};

PyType_Spec NfmDeemphasisSpec = {
    "pycsdr.modules.NfmDeemphasis",
    sizeof(NfmDeemphasis),
    0,
    Py_TPFLAGS_DEFAULT,
    NfmDeemphasisSlots
};
