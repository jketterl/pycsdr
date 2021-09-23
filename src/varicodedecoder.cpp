#include "varicodedecoder.hpp"
#include "types.hpp"

#include <csdr/varicode.hpp>

static int VaricodeDecoder_init(VaricodeDecoder* self, PyObject* args, PyObject* kwds) {
    self->setModule(new Csdr::VaricodeDecoder());

    self->inputFormat = FORMAT_CHAR;
    self->outputFormat = FORMAT_CHAR;

    return 0;
}

static PyType_Slot VaricodeDecoderSlots[] = {
    {Py_tp_init, (void*) VaricodeDecoder_init},
    {0, 0}
};

PyType_Spec VaricodeDecoderSpec = {
    "pycsdr.modules.VaricodeDecoder",
    sizeof(VaricodeDecoder),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    VaricodeDecoderSlots
};
