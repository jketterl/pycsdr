#include "rtty.hpp"
#include "types.hpp"

#include <csdr/rtty.hpp>

static int RttyDecoder_init(RttyDecoder* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "invert", NULL};

    int invert = false;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|p", kwlist, &invert)) {
        return -1;
    }

    self->inputFormat = FORMAT_FLOAT;
    self->outputFormat = FORMAT_CHAR;
    self->setModule(new Csdr::RttyDecoder((bool) invert));

    return 0;
}

static PyType_Slot RttyDecoderSlots[] = {
    {Py_tp_init, (void*) RttyDecoder_init},
    {0, 0}
};

PyType_Spec RttyDecoderSpec = {
    "pycsdr.modules.RttyDecoder",
    sizeof(RttyDecoder),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    RttyDecoderSlots
};
