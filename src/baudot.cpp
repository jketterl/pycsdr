#include "baudot.hpp"
#include "types.hpp"

#include <csdr/baudot.hpp>

static int BaudotDecoder_init(BaudotDecoder* self, PyObject* args, PyObject* kwds) {
    self->inputFormat = FORMAT_CHAR;
    self->outputFormat = FORMAT_CHAR;
    self->setModule(new Csdr::BaudotDecoder());

    return 0;
}

static PyType_Slot BaudotDecoderSlots[] = {
        {Py_tp_init, (void*) BaudotDecoder_init},
        {0, 0}
};

PyType_Spec BaudotDecoderSpec = {
        "pycsdr.modules.BaudotDecoder",
        sizeof(BaudotDecoder),
        0,
        Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
        BaudotDecoderSlots
};
