#include "reader.hpp"

static int Reader_clear(Reader* self) {
    delete self->reader;
    return 0;
}

static PyType_Slot ReaderSlots[] = {
    {Py_tp_clear, (void*) Reader_clear},
    {0, 0}
};

PyType_Spec ReaderSpec {
    "pycsdr.modules.Reader",
    sizeof(Reader),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    ReaderSlots
};