#include "phasedemod.hpp"
#include "types.hpp"

#include <csdr/phasedemod.hpp>

static int PhaseDemod_init(PhaseDemod* self, PyObject* args, PyObject* kwds) {
    self->inputFormat = FORMAT_COMPLEX_FLOAT;
    self->outputFormat = FORMAT_FLOAT;
    self->setModule(new Csdr::PhaseDemod());

    return 0;
}

static PyType_Slot PhaseDemodSlots[] = {
    {Py_tp_init, (void*) PhaseDemod_init},
    {0, 0}
};

PyType_Spec PhaseDemodSpec = {
    "pycsdr.modules.PhaseDemod",
    sizeof(PhaseDemod),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    PhaseDemodSlots
};
