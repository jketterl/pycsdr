#include "downmix.hpp"
#include "types.hpp"
#include <csdr/downmix.hpp>

static int Downmix_init(Downmix* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "format", (char*) "channels", NULL};

    unsigned int channels = 2;
    PyObject* format;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!|I", kwlist, FORMAT_TYPE, &format, &channels)) {
        return -1;
    }

    if (format == FORMAT_SHORT) {
        self->setModule(new Csdr::Downmix<short>(channels));
    } else if (format == FORMAT_FLOAT) {
        self->setModule(new Csdr::Downmix<float>(channels));
    } else {
        PyErr_SetString(PyExc_ValueError, "unsupported downmix format");
        return -1;
    }

    Py_INCREF(format);
    self->inputFormat = format;
    Py_INCREF(format);
    self->outputFormat = format;

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
