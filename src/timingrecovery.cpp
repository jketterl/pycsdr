#include "timingrecovery.hpp"
#include "types.hpp"

#include <csdr/timingrecovery.hpp>

static int TimingRecovery_init(TimingRecovery* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "decimation", (char*) "loopGain", (char*) "maxError", (char*) "useQ", NULL};

    unsigned int decimation = 0;
    float loopGain = 0.0f;
    float maxError = 0.0f;
    int useQ = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "Iffp", kwlist, &decimation, &loopGain, &maxError, &useQ)) {
        return -1;
    }

    self->inputFormat = FORMAT_COMPLEX_FLOAT;
    self->outputFormat = FORMAT_COMPLEX_FLOAT;
    self->setModule(new Csdr::GardnerTimingRecovery(decimation, loopGain, maxError, useQ));

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
