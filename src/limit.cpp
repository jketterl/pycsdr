#include "limit.hpp"
#include "types.hpp"

#include <csdr/limit.hpp>
#include <csdr/window.hpp>

static int Limit_init(Limit* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "maxAmplitude", NULL};

    float maxAmplitude = 1.0f;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|f", kwlist, &maxAmplitude)) {
        return -1;
    }

    self->inputFormat = FORMAT_FLOAT;
    self->outputFormat = FORMAT_FLOAT;
    self->setModule(new Csdr::Limit(maxAmplitude));

    return 0;
}

static PyType_Slot LimitSlots[] = {
    {Py_tp_init, (void*) Limit_init},
    {0, 0}
};

PyType_Spec LimitSpec = {
    "pycsdr.modules.Limit",
    sizeof(Limit),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    LimitSlots
};
