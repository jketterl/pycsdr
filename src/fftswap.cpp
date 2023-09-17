#include "fftswap.hpp"
#include "types.hpp"

#include <csdr/fftexchangesides.hpp>

static int FftSwap_init(FftSwap* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "fft_size", NULL};

    unsigned int fftSize = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "I", kwlist, &fftSize)) {
        return -1;
    }

    self->inputFormat = FORMAT_FLOAT;
    self->outputFormat = FORMAT_FLOAT;
    self->setModule(new Csdr::FftExchangeSides(fftSize));

    return 0;
}

static PyType_Slot FftSwapSlots[] = {
    {Py_tp_init, (void*) FftSwap_init},
    {0, 0}
};

PyType_Spec FftSwapSpec = {
    "pycsdr.modules.FftSwap",
    sizeof(FftSwap),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    FftSwapSlots
};

