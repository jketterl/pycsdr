#include "writer.hpp"

static int Writer_finalize(Writer* self) {
    delete self->writer;
    return 0;
}

static PyType_Slot WriterSlots[] = {
    {Py_tp_finalize, (void*) Writer_finalize},
    {0, 0}
};

PyType_Spec WriterSpec {
    "pycsdr.modules.Writer",
    sizeof(Writer),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_FINALIZE,
    WriterSlots
};