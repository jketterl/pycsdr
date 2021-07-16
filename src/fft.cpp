#include "fft.h"
#include <csdr/fft.hpp>
#include <csdr/window.hpp>

static int Fft_init(Fft* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "size", (char*) "every_n_samples", NULL};

    uint16_t fftSize = 0;
    uint16_t everyNSamples = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "HH", kwlist, &fftSize, &everyNSamples)) {
        return -1;
    }

    // TODO make window available as an argument
    self->module = new Csdr::Fft(fftSize, everyNSamples, new Csdr::HammingWindow());

    return 0;
}

static PyObject* Fft_setEveryNSamples(Fft* self, PyObject* args, PyObject* kwds){
    static char* kwlist[] = {(char*) "every_n_samples", NULL};

    uint16_t everyNSamples = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "H", kwlist, &everyNSamples)) {
        return NULL;
    }

    ((Csdr::Fft*) self->module)->setEveryNSamples(everyNSamples);

    Py_RETURN_NONE;
}

static PyMethodDef Fft_methods[] = {
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
    {"setEveryNSamples", (PyCFunction) Fft_setEveryNSamples, METH_VARARGS | METH_KEYWORDS,
     "set repetition interval in samples"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot FftSlots[] = {
    {Py_tp_init, (void*) Fft_init},
    {Py_tp_clear, (void*) Module_clear<Csdr::complex<float>, Csdr::complex<float>>},
    {Py_tp_methods, Fft_methods},
    {0, 0}
};

PyType_Spec FftSpec = {
    "pycsdr.modules.Fft",
    sizeof(Fft),
    0,
    Py_TPFLAGS_DEFAULT,
    FftSlots
};
