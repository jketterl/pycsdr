#include "fftadpcm.hpp"
#include "types.hpp"

#include <csdr/adpcm.hpp>

static int FftAdpcm_init(FftAdpcm* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "fft_size", NULL};

    unsigned int fftSize = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "I", kwlist, &fftSize)) {
        return -1;
    }

    self->inputFormat = FORMAT_FLOAT;
    self->outputFormat = FORMAT_CHAR;

    self->setModule(new Csdr::FftAdpcmEncoder(fftSize));

    return 0;
}

static PyType_Slot FftAdpcmSlots[] = {
    {Py_tp_init, (void*) FftAdpcm_init},
    {0, 0}
};

PyType_Spec FftAdpcmSpec = {
    "pycsdr.modules.FftAdpcm",
    sizeof(FftAdpcm),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    FftAdpcmSlots
};
