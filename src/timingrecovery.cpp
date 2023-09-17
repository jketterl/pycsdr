#include "timingrecovery.hpp"
#include "types.hpp"

#include <csdr/timingrecovery.hpp>

static int TimingRecovery_init(TimingRecovery* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "format", (char*) "decimation", (char*) "loopGain", (char*) "maxError", NULL};

    PyObject* format;
    unsigned int decimation = 0;
    float loopGain = 0.0f;
    float maxError = 0.0f;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!Iff", kwlist, FORMAT_TYPE, &format, &decimation, &loopGain, &maxError)) {
        return -1;
    }

    if (format == FORMAT_FLOAT) {
        self->setModule(new Csdr::GardnerTimingRecovery<float>(decimation, loopGain, maxError));
    } else if (format == FORMAT_COMPLEX_FLOAT) {
        self->setModule(new Csdr::GardnerTimingRecovery<Csdr::complex<float>>(decimation, loopGain, maxError));
    } else {
        PyErr_SetString(PyExc_ValueError, "unsupported timing recovery format");
        return -1;
    }

    Py_INCREF(format);
    self->inputFormat = format;
    Py_INCREF(format);
    self->outputFormat = format;

    return 0;
}

static PyType_Slot TimingRecoverySlots[] = {
    {Py_tp_init, (void*) TimingRecovery_init},
    {0, 0}
};

PyType_Spec TimingRecoverySpec = {
    "pycsdr.modules.TimingRecovery",
    sizeof(TimingRecovery),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    TimingRecoverySlots
};
