#include "opusencoder.hpp"
#include "types.hpp"
#include <csdr/opus.hpp>

static int OpusEncoder_init(OpusEncoder* self, PyObject* args, PyObject* kwds) {
    self->inputFormat = FORMAT_SHORT;
    self->outputFormat = FORMAT_CHAR;
    self->setModule(new Csdr::OpusEncoder());

    return 0;
}

static PyType_Slot OpusEncoderSlots[] = {
    {Py_tp_init, (void*) OpusEncoder_init},
    {0, 0}
};

PyType_Spec OpusEncoderSpec = {
    "pycsdr.modules.OpusEncoder",
    sizeof(OpusEncoder),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    OpusEncoderSlots
};
