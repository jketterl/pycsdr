#include "reader.hpp"

static int Reader_finalize(Reader* self) {
    delete self->reader;
    return 0;
}

static PyType_Slot ReaderSlots[] = {
    {Py_tp_finalize, (void*) Reader_finalize},
    {0, 0}
};

PyType_Spec ReaderSpec {
    "pycsdr.modules.Reader",
    sizeof(Reader),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_FINALIZE,
    ReaderSlots
};