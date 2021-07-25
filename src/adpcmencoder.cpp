#include "adpcmencoder.hpp"
#include "types.h"
#include <csdr/adpcm.hpp>

static int AdpcmEncoder_init(AdpcmEncoder* self, PyObject* args, PyObject* kwds) {
    self->inputFormat = FORMAT_SHORT;
    self->outputFormat = FORMAT_CHAR;
    self->setModule(new Csdr::AdpcmEncoder(false));

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
    Py_TPFLAGS_DEFAULT,
    AdpcmEncoderSlots
};
