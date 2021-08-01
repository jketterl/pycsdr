#include "dcblock.hpp"
#include "types.hpp"

#include <csdr/dcblock.hpp>

static int DcBlock_init(DcBlock* self, PyObject* args, PyObject* kwds) {
    self->setModule(new Csdr::DcBlock());

    Py_INCREF(FORMAT_FLOAT);
    self->inputFormat = FORMAT_FLOAT;
    self->outputFormat = FORMAT_FLOAT;

    return 0;
}

static PyType_Slot DcBlockSlots[] = {
    {Py_tp_init, (void*) DcBlock_init},
    {0, 0}
};

PyType_Spec DcBlockSpec = {
    "pycsdr.modules.DcBlock",
    sizeof(DcBlock),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    DcBlockSlots
};
