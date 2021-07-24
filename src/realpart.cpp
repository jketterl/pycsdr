#include "realpart.h"
#include "types.h"

#include <csdr/realpart.hpp>

static int RealPart_init(RealPart* self, PyObject* args, PyObject* kwds) {
    self->inputFormat = FORMAT_COMPLEX_FLOAT;
    self->outputFormat = FORMAT_FLOAT;
    self->setModule(new Csdr::Realpart());

    return 0;
}

static PyType_Slot RealPartSlots[] = {
    {Py_tp_init, (void*) RealPart_init},
    {0, 0}
};

PyType_Spec RealPartSpec = {
    "pycsdr.modules.RealPart",
    sizeof(RealPart),
    0,
    Py_TPFLAGS_DEFAULT,
    RealPartSlots
};
