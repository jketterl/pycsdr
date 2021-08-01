#include "adpcmencoder.hpp"
#include "types.hpp"
#include <csdr/adpcm.hpp>

static int AdpcmEncoder_init(AdpcmEncoder* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "sync", NULL};

    bool sync = false;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|b", kwlist, &sync)) {
        return -1;
    }

    self->inputFormat = FORMAT_SHORT;
    self->outputFormat = FORMAT_CHAR;
    self->setModule(new Csdr::AdpcmEncoder(sync));

    return 0;
}

static PyType_Slot AdpcmEncoderSlots[] = {
    {Py_tp_init, (void*) AdpcmEncoder_init},
    {0, 0}
};

PyType_Spec AdpcmEncoderSpec = {
    "pycsdr.modules.AdpcmEncoder",
    sizeof(AdpcmEncoder),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    AdpcmEncoderSlots
};
