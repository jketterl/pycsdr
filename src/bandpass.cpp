#include "bandpass.hpp"
#include "types.hpp"

#include <csdr/filter.hpp>
#include <csdr/fir.hpp>
#include <csdr/fftfilter.hpp>
#include <csdr/window.hpp>

static int Bandpass_init(Bandpass* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "low_cut", (char*) "high_cut", (char*) "transition", (char*) "use_fft", NULL};

    int use_fft = true;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|fffp", kwlist, &self->low_cut, &self->high_cut, &self->transition, &use_fft)) {
        return -1;
    }
    self->use_fft = use_fft;

    Csdr::Filter<Csdr::complex<float>>* filter;
    auto window = new Csdr::HammingWindow();
    if (self->use_fft) {
        filter = new Csdr::FftBandPassFilter(self->low_cut, self->high_cut, self->transition, window);
    } else {
        filter = new Csdr::BandPassFilter<Csdr::complex<float>>(self->low_cut, self->high_cut, self->transition, window);
    }
    self->setModule(new Csdr::FilterModule<Csdr::complex<float>>(filter));
    delete window;

    self->inputFormat = FORMAT_COMPLEX_FLOAT;
    self->outputFormat = FORMAT_COMPLEX_FLOAT;

    return 0;
}

static PyObject* Bandpass_setBandpass(Bandpass* self, PyObject* args, PyObject* kwds){
    static char* kwlist[] = {(char*) "low_cut", (char*) "high_cut", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "ff", kwlist, &self->low_cut, &self->high_cut)) {
        return NULL;
    }

    Csdr::Filter<Csdr::complex<float>>* filter;
    auto window = new Csdr::HammingWindow();
    if (self->use_fft) {
        filter = new Csdr::FftBandPassFilter(self->low_cut, self->high_cut, self->transition, window);
    } else {
        filter = new Csdr::BandPassFilter<Csdr::complex<float>>(self->low_cut, self->high_cut, self->transition, window);
    }

    dynamic_cast<Csdr::FilterModule<Csdr::complex<float>>*>(self->module)->setFilter(filter);
    delete window;

    Py_RETURN_NONE;
}

static PyMethodDef Bandpass_methods[] = {
    {"setBandpass", (PyCFunction) Bandpass_setBandpass, METH_VARARGS | METH_KEYWORDS,
     "set bandpass cutoffs"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot BandpassSlots[] = {
    {Py_tp_init, (void*) Bandpass_init},
    {Py_tp_methods, Bandpass_methods},
    {0, 0}
};

PyType_Spec BandpassSpec = {
    "pycsdr.modules.Bandpass",
    sizeof(Bandpass),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    BandpassSlots
};
