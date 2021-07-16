#include "bandpass.h"

#include <csdr/filter.hpp>
#include <csdr/fir.hpp>
#include <csdr/fftfilter.hpp>
#include <csdr/window.hpp>

static int Bandpass_init(Bandpass* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "low_cut", (char*) "high_cut", (char*) "transition", (char*) "use_fft", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|fffp", kwlist, &self->low_cut, self->high_cut, self->transition, self->use_fft)) {
        return -1;
    }

    Csdr::Filter<Csdr::complex<float>>* filter;
    if (self->use_fft) {
        filter = new Csdr::FftBandPassFilter(self->low_cut, self->high_cut, self->transition, new Csdr::HammingWindow());
    } else {
        filter = new Csdr::BandPassFilter<Csdr::complex<float>>(self->low_cut, self->high_cut, self->transition, new Csdr::HammingWindow());
    }
    self->module = new Csdr::FilterModule<Csdr::complex<float>>(filter);

    return 0;
}

static int Bandpass_clear(Bandpass* self) {
    Module_stop(self);

    delete self->module;

    return 0;
}

static PyObject* Bandpass_setBandpass(Bandpass* self, PyObject* args, PyObject* kwds){
    static char* kwlist[] = {(char*) "low_cut", (char*) "high_cut", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "f", kwlist, &self->low_cut, &self->low_cut)) {
        return NULL;
    }

    Csdr::Filter<Csdr::complex<float>>* filter;
    if (self->use_fft) {
        filter = new Csdr::FftBandPassFilter(self->low_cut, self->high_cut, self->transition, new Csdr::HammingWindow());
    } else {
        filter = new Csdr::BandPassFilter<Csdr::complex<float>>(self->low_cut, self->high_cut, self->transition, new Csdr::HammingWindow());
    }

    ((Csdr::FilterModule<Csdr::complex<float>>*) self->module)->setFilter(filter);

    Py_RETURN_NONE;
}

static PyMethodDef Bandpass_methods[] = {
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
    {"setBandpass", (PyCFunction) Bandpass_setBandpass, METH_VARARGS | METH_KEYWORDS,
     "set bandpass cutoffs"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot BandpassSlots[] = {
    {Py_tp_init, (void*) Bandpass_init},
    {Py_tp_clear, (void*) Bandpass_clear},
    {Py_tp_methods, Bandpass_methods},
    {0, 0}
};

PyType_Spec BandpassSpec = {
    "pycsdr.modules.Bandpass",
    sizeof(Bandpass),
    0,
    Py_TPFLAGS_DEFAULT,
    BandpassSlots
};
