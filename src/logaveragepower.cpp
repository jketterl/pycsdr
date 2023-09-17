#include "logaveragepower.hpp"
#include "types.hpp"

#include <csdr/logaveragepower.hpp>

static int LogAveragePower_init(LogAveragePower* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "add_db", (char*) "fft_size", (char*) "avg_number", NULL};

    float add_db = 0.0;
    unsigned int fftSize = 0;
    unsigned int avgNumber = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "fII", kwlist, &add_db, &fftSize, &avgNumber)) {
        return -1;
    }

    self->inputFormat = FORMAT_COMPLEX_FLOAT;
    self->outputFormat = FORMAT_FLOAT;
    self->setModule(new Csdr::LogAveragePower(fftSize, avgNumber, add_db));

    return 0;
}

static PyObject* LogAveragePower_setFftAverages(LogAveragePower* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "avg_number", NULL};

    uint16_t avgNumber = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "H", kwlist, &avgNumber)) {
        return NULL;
    }

    dynamic_cast<Csdr::LogAveragePower*>(self->module)->setAvgNumber(avgNumber);

    Py_RETURN_NONE;
}

static PyMethodDef LogAveragePower_methods[] = {
    {"setAvgNumber", (PyCFunction) LogAveragePower_setFftAverages, METH_VARARGS | METH_KEYWORDS,
     "set fft averageing factor"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot LogAveragePowerSlots[] = {
    {Py_tp_init, (void*) LogAveragePower_init},
    {Py_tp_methods, LogAveragePower_methods},
    {0, 0}
};

PyType_Spec LogAveragePowerSpec = {
    "pycsdr.modules.LogAveragePower",
    sizeof(LogAveragePower),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    LogAveragePowerSlots
};
