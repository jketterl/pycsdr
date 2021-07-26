#include "audioresampler.hpp"
#include "types.hpp"
#include <csdr/audioresampler.hpp>

static int AudioResampler_init(AudioResampler* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "inputRate", (char*) "outputRate", NULL};

    unsigned int inputRate = 0;
    unsigned int outputRate = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "II", kwlist, &inputRate, &outputRate)) {
        return -1;
    }

    self->inputFormat = FORMAT_FLOAT;
    self->outputFormat = FORMAT_FLOAT;
    self->setModule(new Csdr::AudioResampler(inputRate, outputRate));

    return 0;
}

static PyType_Slot AudioResamplerSlots[] = {
        {Py_tp_init, (void*) AudioResampler_init},
        {0, 0}
};

PyType_Spec AudioResamplerSpec = {
        "pycsdr.modules.AudioResampler",
        sizeof(AudioResampler),
        0,
        Py_TPFLAGS_DEFAULT,
        AudioResamplerSlots
};
