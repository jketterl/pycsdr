#include "amdemod.hpp"
#include "types.hpp"
#include <csdr/amdemod.hpp>

static int AmDemod_init(AmDemod* self, PyObject* args, PyObject* kwds) {
    self->inputFormat = FORMAT_COMPLEX_FLOAT;
    self->outputFormat = FORMAT_FLOAT;
    self->setModule(new Csdr::AmDemod());

    return 0;
}

static PyType_Slot AmDemodSlots[] = {
    {Py_tp_init, (void*) AmDemod_init},
    {0, 0}
};

PyType_Spec AmDemodSpec = {
    "pycsdr.modules.AmDemod",
    sizeof(AmDemod),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    AmDemodSlots
};
