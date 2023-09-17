#include "firdecimate.hpp"
#include "types.hpp"

#include <csdr/firdecimate.hpp>
#include <csdr/window.hpp>

static int FirDecimate_init(FirDecimate* self, PyObject* args, PyObject* kwds) {

    float transition = 0.05f;
    unsigned int decimation = 0;
    float cutoff = 0.5f;

    // TODO restore window argument
    static char* kwlist[] = {(char*) "decimation", (char*) "transition", (char*) "cutoff", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "I|ff", kwlist, &decimation, &transition, &cutoff)) {
        return -1;
    }

    self->inputFormat = FORMAT_COMPLEX_FLOAT;
    self->outputFormat = FORMAT_COMPLEX_FLOAT;
    auto window = new Csdr::HammingWindow();
    self->setModule(new Csdr::FirDecimate(decimation, transition, window, cutoff));
    delete window;

    return 0;
}

static PyType_Slot FirDecimateSlots[] = {
    {Py_tp_init, (void*) FirDecimate_init},
    {0, 0}
};

PyType_Spec FirDecimateSpec = {
    "pycsdr.modules.FirDecimate",
    sizeof(FirDecimate),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    FirDecimateSlots
};

