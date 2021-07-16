#include "fftswap.h"

#include <csdr/fftexchangesides.hpp>

static int FftSwap_init(FftSwap* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "fft_size", NULL};

    uint16_t fftSize = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "H", kwlist, &fftSize)) {
        return -1;
    }

    self->module = new Csdr::FftExchangeSides(fftSize);

    return 0;
}

static int FftSwap_clear(FftSwap* self) {
    Module_stop(self);

    delete self->module;

    return 0;
}


static PyMethodDef FftSwap_methods[] = {
    {"setInput", (PyCFunction) Module_setInput<float, float>, METH_VARARGS | METH_KEYWORDS,
     "set the input buffer"
    },
    {"setOutput", (PyCFunction) Module_setOutput<float, float>, METH_VARARGS | METH_KEYWORDS,
     "set the output buffer"
    },
    {"getOutputFormat", (PyCFunction) Module_getOutputFormat<float, float>, METH_NOARGS,
     "get output format"
    },
    {"stop", (PyCFunction) Module_stop<float, float>, METH_NOARGS,
     "stop processing"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot FftSwapSlots[] = {
    {Py_tp_init, (void*) FftSwap_init},
    {Py_tp_clear, (void*) FftSwap_clear},
    {Py_tp_methods, FftSwap_methods},
    {0, 0}
};

PyType_Spec FftSwapSpec = {
    "pycsdr.modules.FftSwap",
    sizeof(FftSwap),
    0,
    Py_TPFLAGS_DEFAULT,
    FftSwapSlots
};

