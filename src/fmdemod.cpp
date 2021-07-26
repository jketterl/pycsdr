#include "fmdemod.hpp"
#include "types.hpp"

#include <csdr/fmdemod.hpp>

static int FmDemod_init(FmDemod* self, PyObject* args, PyObject* kwds) {
    self->inputFormat = FORMAT_COMPLEX_FLOAT;
    self->outputFormat = FORMAT_FLOAT;
    self->setModule(new Csdr::FmDemod());

    return 0;
}

static PyType_Slot FmDemodSlots[] = {
    {Py_tp_init, (void*) FmDemod_init},
    {0, 0}
};

PyType_Spec FmDemodSpec = {
    "pycsdr.modules.FmDemod",
    sizeof(FmDemod),
    0,
    Py_TPFLAGS_DEFAULT,
    FmDemodSlots
};
