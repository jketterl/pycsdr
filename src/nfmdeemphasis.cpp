#include "nfmdeemphasis.hpp"
#include "types.hpp"

#include <csdr/deemphasis.hpp>

static int NfmDeemphasis_init(NfmDeemphasis* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "sampleRate", NULL};

    unsigned int sampleRate = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "I", kwlist, &sampleRate)) {
        return -1;
    }

    self->inputFormat = FORMAT_FLOAT;
    self->outputFormat = FORMAT_FLOAT;
    Csdr::NfmDeephasis* module;
    try {
        module = new Csdr::NfmDeephasis(sampleRate);
    } catch (const std::runtime_error& e) {
        PyErr_SetString(PyExc_ValueError, e.what());
        return -1;
    }
    self->setModule(module);

    return 0;
}

static PyType_Slot NfmDeemphasisSlots[] = {
    {Py_tp_init, (void*) NfmDeemphasis_init},
    {0, 0}
};

PyType_Spec NfmDeemphasisSpec = {
    "pycsdr.modules.NfmDeemphasis",
    sizeof(NfmDeemphasis),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    NfmDeemphasisSlots
};
