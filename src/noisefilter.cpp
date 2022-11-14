#include "noisefilter.hpp"
#include "types.hpp"

#include <csdr/filter.hpp>
#include <csdr/noisefilter.hpp>

static int NoiseFilter_init(NoiseFilter* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {
      (char*)"threshold", (char*)"wndSize", (char*)"fftSize", NULL
    };

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|ddd", kwlist, &self->threshold, &self->wndSize, &self->fftSize)) {
        return -1;
    }

    Csdr::Filter<float>* filter =
        new Csdr::AFNoiseFilter(self->threshold, self->wndSize, self->fftSize);

    self->setModule(new Csdr::FilterModule<float>(filter));

    Py_INCREF(FORMAT_FLOAT);
    self->inputFormat = FORMAT_FLOAT;
    self->outputFormat = FORMAT_FLOAT;

    return 0;
}

static PyObject* NoiseFilter_setThreshold(NoiseFilter* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = { (char*)"threshold", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "d", kwlist, &self->threshold)) {
        return NULL;
    }

    Csdr::Filter<float>* filter =
        new Csdr::AFNoiseFilter(self->threshold, self->wndSize, self->fftSize);

    dynamic_cast<Csdr::FilterModule<float>*>(self->module)->setFilter(filter);

    Py_RETURN_NONE;
}

static PyObject* NoiseFilter_setWndSize(NoiseFilter* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = { (char*)"wndSize", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "d", kwlist, &self->wndSize)) {
        return NULL;
    }

    Csdr::Filter<float>* filter =
        new Csdr::AFNoiseFilter(self->threshold, self->wndSize, self->fftSize);

    dynamic_cast<Csdr::FilterModule<float>*>(self->module)->setFilter(filter);

    Py_RETURN_NONE;
}

static PyObject* NoiseFilter_setFftSize(NoiseFilter* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = { (char*)"fftSize", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "d", kwlist, &self->fftSize)) {
        return NULL;
    }

    Csdr::Filter<float>* filter =
        new Csdr::AFNoiseFilter(self->threshold, self->wndSize, self->fftSize);

    dynamic_cast<Csdr::FilterModule<float>*>(self->module)->setFilter(filter);

    Py_RETURN_NONE;
}

static PyMethodDef NoiseFilter_methods[] = {
    { "setThreshold", (PyCFunction) NoiseFilter_setThreshold, METH_VARARGS | METH_KEYWORDS, "set filtering threshold in dB" },
    { "setWndSize", (PyCFunction) NoiseFilter_setWndSize, METH_VARARGS | METH_KEYWORDS, "set window size in buckets" },
    { "setFftSize", (PyCFunction) NoiseFilter_setFftSize, METH_VARARGS | METH_KEYWORDS, "set FFT size in buckets" },
    { NULL }  /* Sentinel */
};

static PyType_Slot NoiseFilterSlots[] = {
    { Py_tp_init, (void*) NoiseFilter_init },
    { Py_tp_methods, NoiseFilter_methods },
    { 0, 0}
};

PyType_Spec NoiseFilterSpec = {
    "pycsdr.modules.NoiseFilter",
    sizeof(NoiseFilter),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    NoiseFilterSlots
};
