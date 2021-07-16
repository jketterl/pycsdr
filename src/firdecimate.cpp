#include "firdecimate.h"

#include <csdr/firdecimate.hpp>
#include <csdr/window.hpp>

static int FirDecimate_init(FirDecimate* self, PyObject* args, PyObject* kwds) {

    float transition = 0.05;
    unsigned int decimation = 0;

    // TODO restore window argument
    static char* kwlist[] = {(char*) "decimation", (char*) "transition", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "H|f", kwlist, &decimation, &transition)) {
        return -1;
    }

    self->module = new Csdr::FirDecimate(decimation, transition, new Csdr::HammingWindow());

    return 0;
}

static PyMethodDef FirDecimate_methods[] = {
    {"setInput", (PyCFunction) Module_setInput<Csdr::complex<float>, Csdr::complex<float>>, METH_VARARGS | METH_KEYWORDS,
     "set the input buffer"
    },
    {"setOutput", (PyCFunction) Module_setOutput<Csdr::complex<float>, Csdr::complex<float>>, METH_VARARGS | METH_KEYWORDS,
     "set the output buffer"
    },
    {"getOutputFormat", (PyCFunction) Module_getOutputFormat<Csdr::complex<float>, Csdr::complex<float>>, METH_NOARGS,
     "get output format"
    },
    {"stop", (PyCFunction) Module_stop<Csdr::complex<float>, Csdr::complex<float>>, METH_NOARGS,
     "stop processing"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot FirDecimateSlots[] = {
    {Py_tp_init, (void*) FirDecimate_init},
    {Py_tp_clear, (void*) Module_clear<Csdr::complex<float>, Csdr::complex<float>>},
    {Py_tp_methods, FirDecimate_methods},
    {0, 0}
};

PyType_Spec FirDecimateSpec = {
    "pycsdr.modules.FirDecimate",
    sizeof(FirDecimate),
    0,
    Py_TPFLAGS_DEFAULT,
    FirDecimateSlots
};

