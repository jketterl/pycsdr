#include "lowpass.hpp"
#include "types.hpp"

#include <csdr/filter.hpp>
#include <csdr/fir.hpp>

static int Lowpass_init(Lowpass* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "format", (char*) "cutoff", (char*) "transition", NULL};

    float cutoff = 0.0f;
    float transition = 0.05f;
    PyObject* format;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!f|f", kwlist, FORMAT_TYPE, &format, &cutoff, &transition)) {
        return -1;
    }

    if (format == FORMAT_FLOAT) {
        auto window = new Csdr::HammingWindow();
        self->setModule(new Csdr::FilterModule<float>(new Csdr::LowPassFilter<float>(cutoff, transition, window)));
    } else if (format == FORMAT_COMPLEX_FLOAT) {
        auto window = new Csdr::HammingWindow();
        self->setModule(new Csdr::FilterModule<Csdr::complex<float>>(new Csdr::LowPassFilter<Csdr::complex<float>>(cutoff, transition, window)));
    } else {
        PyErr_SetString(PyExc_ValueError, "unsupported lowpass format");
        return -1;
    }

    Py_INCREF(format);
    self->inputFormat = format;
    Py_INCREF(format);
    self->outputFormat = format;

    return 0;
}

static PyType_Slot LowpassSlots[] = {
    {Py_tp_init, (void*) Lowpass_init},
    {0, 0}
};

PyType_Spec LowpassSpec = {
    "pycsdr.modules.Lowpass",
    sizeof(Lowpass),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    LowpassSlots
};
