#include "wfmdeemphasis.hpp"
#include "types.hpp"

#include <csdr/deemphasis.hpp>

static int WfmDeemphasis_init(WfmDeemphasis* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "sampleRate", (char*) "tau", NULL};

    unsigned int sampleRate = 0;
    float tau = 0.0f;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "If", kwlist, &sampleRate, &tau)) {
        return -1;
    }

    self->inputFormat = FORMAT_FLOAT;
    self->outputFormat = FORMAT_FLOAT;
    self->setModule(new Csdr::WfmDeemphasis(sampleRate, tau));

    return 0;
}

static PyType_Slot WfmDeemphasisSlots[] = {
        {Py_tp_init, (void*) WfmDeemphasis_init},
        {0, 0}
};

PyType_Spec WfmDeemphasisSpec = {
        "pycsdr.modules.WfmDeemphasis",
        sizeof(WfmDeemphasis),
        0,
        Py_TPFLAGS_DEFAULT,
        WfmDeemphasisSlots
};
