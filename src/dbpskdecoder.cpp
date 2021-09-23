#include "dbpskdecoder.hpp"
#include "types.hpp"

#include <csdr/dbpsk.hpp>

static int DBPskDecoder_init(DBPskDecoder* self, PyObject* args, PyObject* kwds) {
    self->setModule(new Csdr::DBPskDecoder());

    self->inputFormat = FORMAT_COMPLEX_FLOAT;
    self->outputFormat = FORMAT_CHAR;

    return 0;
}

static PyType_Slot DBPskDecoderSlots[] = {
    {Py_tp_init, (void*) DBPskDecoder_init},
    {0, 0}
};

PyType_Spec DBPskDecoderSpec = {
    "pycsdr.modules.DBPskDecoder",
    sizeof(DBPskDecoder),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    DBPskDecoderSlots
};
