#include "fftadpcm.h"

#include <csdr/adpcm.hpp>

static int FftAdpcm_init(FftAdpcm* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "fft_size", NULL};

    uint16_t fftSize = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "H", kwlist, &fftSize)) {
        return -1;
    }

    self->module = new Csdr::FftAdpcmEncoder(fftSize);

    return 0;
}

static PyMethodDef FftAdpcm_methods[] = {
    {"setInput", (PyCFunction) Module_setInput<float, unsigned char>, METH_VARARGS | METH_KEYWORDS,
     "set the input buffer"
    },
    {"setOutput", (PyCFunction) Module_setOutput<float, unsigned char>, METH_VARARGS | METH_KEYWORDS,
     "set the output buffer"
    },
    {"getOutputFormat", (PyCFunction) Module_getOutputFormat<unsigned char>, METH_NOARGS,
     "get output format"
    },
    {"stop", (PyCFunction) Module_stop, METH_NOARGS,
     "stop processing"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot FftAdpcmSlots[] = {
    {Py_tp_init, (void*) FftAdpcm_init},
    {Py_tp_clear, (void*) Module_clear<float>},
    {Py_tp_methods, FftAdpcm_methods},
    {0, 0}
};

PyType_Spec FftAdpcmSpec = {
    "pycsdr.modules.FftAdpcm",
    sizeof(FftAdpcm),
    0,
    Py_TPFLAGS_DEFAULT,
    FftAdpcmSlots
};
