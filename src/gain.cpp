#include "gain.hpp"
#include "types.hpp"

#include <csdr/gain.hpp>

static int Gain_init(Gain* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "format", (char*) "gain", NULL};

    float gain = 0.0f;
    PyObject* format;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!f", kwlist, FORMAT_TYPE, &format, &gain)) {
        return -1;
    }

    if (format == FORMAT_FLOAT) {
        self->setModule(new Csdr::Gain<float>(gain));
    } else if (format == FORMAT_COMPLEX_FLOAT) {
        self->setModule(new Csdr::Gain<Csdr::complex<float>>(gain));
    } else {
        PyErr_SetString(PyExc_ValueError, "unsupported gain format");
        return -1;
    }

    Py_INCREF(format);
    self->inputFormat = format;
    Py_INCREF(format);
    self->outputFormat = format;

    return 0;
}

static PyType_Slot GainSlots[] = {
    {Py_tp_init, (void*) Gain_init},
    {0, 0}
};

PyType_Spec GainSpec = {
    "pycsdr.modules.Gain",
    sizeof(Gain),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    GainSlots
};
