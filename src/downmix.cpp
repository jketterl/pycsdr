#include "downmix.hpp"
#include "types.hpp"
#include <csdr/downmix.hpp>

static int Downmix_init(Downmix* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "channels", NULL};

    unsigned int channels = 2;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|I", kwlist, &channels)) {
        return -1;
    }

    self->inputFormat = FORMAT_SHORT;
    self->outputFormat = FORMAT_SHORT;
    self->setModule(new Csdr::Downmix<short>(channels));

    return 0;
}

static PyType_Slot DownmixSlots[] = {
    {Py_tp_init, (void*) Downmix_init},
    {0, 0}
};

PyType_Spec DownmixSpec = {
    "pycsdr.modules.Downmix",
    sizeof(Downmix),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    DownmixSlots
};
