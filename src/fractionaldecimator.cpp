#include "fractionaldecimator.hpp"
#include "types.hpp"

#include <csdr/fractionaldecimator.hpp>
#include <csdr/window.hpp>

template <typename T>
static void setupDecimator(FractionalDecimator* self, float decimation, unsigned int numPolyPoints, bool prefilter) {
    Csdr::LowPassFilter<T>* filter = nullptr;
    if (prefilter) {
        auto w = new Csdr::HammingWindow();
        float transition = 0.03f;
        filter = new Csdr::LowPassFilter<T>(0.5 / (decimation - transition), transition, w);
    }
    self->setModule(new Csdr::FractionalDecimator<T>(decimation, numPolyPoints, filter));
}

static int FractionalDecimator_init(FractionalDecimator* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "format", (char*) "decimation", (char*) "numPolyPoints", (char*) "prefilter", NULL};

    PyObject* format;
    float decimation = 0.0f;
    unsigned int numPolyPoints = 12;
    int prefilter = false;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!f|Ip", kwlist, FORMAT_TYPE, &format, &decimation, &numPolyPoints, &prefilter)) {
        return -1;
    }

    if (format == FORMAT_FLOAT) {
        setupDecimator<float>(self, decimation, numPolyPoints, prefilter);
    } else if (format == FORMAT_COMPLEX_FLOAT) {
        setupDecimator<Csdr::complex<float>>(self, decimation, numPolyPoints, prefilter);
    } else {
        PyErr_SetString(PyExc_ValueError, "unsupported fractional decimator format");
        return -1;
    }

    Py_INCREF(format);
    self->inputFormat = format;
    Py_INCREF(format);
    self->outputFormat = format;

    return 0;
}

static PyType_Slot FractionalDecimatorSlots[] = {
    {Py_tp_init, (void*) FractionalDecimator_init},
    {0, 0}
};

PyType_Spec FractionalDecimatorSpec = {
    "pycsdr.modules.FractionalDecimator",
    sizeof(FractionalDecimator),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    FractionalDecimatorSlots
};
