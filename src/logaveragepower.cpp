#include "logaveragepower.h"

#include <csdr/logaveragepower.hpp>

static int LogAveragePower_init(LogAveragePower* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "add_db", (char*) "fft_size", (char*) "avg_number", NULL};

    float add_db = 0.0;
    uint16_t fftSize = 0;
    uint16_t avgNumber = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "fHH", kwlist, &add_db, &fftSize, &avgNumber)) {
        return -1;
    }

    self->module = new Csdr::LogAveragePower(fftSize, avgNumber, add_db);

    return 0;
}

static int LogAveragePower_clear(LogAveragePower* self) {
    Module_stop(self);

    delete self->module;

    return 0;
}

static PyObject* LogAveragePower_setFftAverages(LogAveragePower* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "avg_number", NULL};

    uint16_t avgNumber = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "H", kwlist, &avgNumber)) {
        return NULL;
    }

    ((Csdr::LogAveragePower*) self->module)->setAvgNumber(avgNumber);

    Py_RETURN_NONE;
}

static PyMethodDef LogAveragePower_methods[] = {
    {"setInput", (PyCFunction) Module_setInput<Csdr::complex<float>, float>, METH_VARARGS | METH_KEYWORDS,
     "set the input buffer"
    },
    {"setOutput", (PyCFunction) Module_setOutput<Csdr::complex<float>, float>, METH_VARARGS | METH_KEYWORDS,
     "set the output buffer"
    },
    {"getOutputFormat", (PyCFunction) Module_getOutputFormat<Csdr::complex<float>, float>, METH_NOARGS,
     "get output format"
    },
    {"stop", (PyCFunction) Module_stop<Csdr::complex<float>, float>, METH_NOARGS,
     "stop processing"
    },
    {"setAvgNumber", (PyCFunction) LogAveragePower_setFftAverages, METH_VARARGS | METH_KEYWORDS,
     "set fft averageing factor"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot LogAveragePowerSlots[] = {
    {Py_tp_init, (void*) LogAveragePower_init},
    {Py_tp_clear, (void*) LogAveragePower_clear},
    {Py_tp_methods, LogAveragePower_methods},
    {0, 0}
};

PyType_Spec LogAveragePowerSpec = {
    "pycsdr.modules.LogAveragePower",
    sizeof(LogAveragePower),
    0,
    Py_TPFLAGS_DEFAULT,
    LogAveragePowerSlots
};
