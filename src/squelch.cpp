#include "squelch.h"
#include "types.h"
#include "pycsdr.hpp"

#include <csdr/power.hpp>

static int Squelch_init(Squelch* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "decimation", (char*) "reportInterval", NULL};

    unsigned int decimation = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "II", kwlist, &decimation, &self->reportInterval)) {
        return -1;
    }

    self->inputFormat = FORMAT_COMPLEX_FLOAT;
    self->outputFormat = FORMAT_COMPLEX_FLOAT;
    self->reportCounter = self->reportInterval;
    self->setModule(new Csdr::Squelch(decimation, [self] (float level) {
        if (self->reportCounter-- > 0) return;
        self->reportCounter = self->reportInterval;

        if (self->powerWriter == nullptr) return;

        auto writer = dynamic_cast<Csdr::Writer<float>*>(self->powerWriter->writer);
        if (!writer->writeable()) return;
        *(writer->getWritePointer()) = level;
        writer->advance(1);
    }));

    return 0;
}

static PyObject* Squelch_setSquelchLevel(Squelch* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "level", NULL};

    float level = 0.0f;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "f", kwlist, &level)) {
        return NULL;
    }

    dynamic_cast<Csdr::Squelch*>(self->module)->setSquelch(level);

    Py_RETURN_NONE;
}

static PyObject* Squelch_setPowerWriter(Squelch* self, PyObject* args, PyObject* kwds) {
    Writer* writer;

    static char* kwlist[] = {(char*) "writer", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist, WriterType, &writer)) {
        return NULL;
    }

    if ((PyObject*) writer != Py_None && writer->writerFormat != FORMAT_FLOAT) {
        PyErr_SetString(PyExc_ValueError, "invalid writer format");
        return NULL;
    }

    if (self->powerWriter != nullptr) {
        Py_DECREF(self->powerWriter);
        self->powerWriter = nullptr;
    }

    if ((PyObject*) writer != Py_None) {
        self->powerWriter = writer;
        Py_INCREF(self->powerWriter);
    }

    Py_RETURN_NONE;
}

static PyMethodDef Squelch_methods[] = {
    {"setSquelchLevel", (PyCFunction) Squelch_setSquelchLevel, METH_VARARGS | METH_KEYWORDS,
     "set squelch level in dB"
    },
    {"setPowerWriter", (PyCFunction) Squelch_setPowerWriter, METH_VARARGS | METH_KEYWORDS,
     "set a writer that will receive power level readouts"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot SquelchSlots[] = {
    {Py_tp_init, (void*) Squelch_init},
    {Py_tp_methods, Squelch_methods},
    {0, 0}
};

PyType_Spec SquelchSpec = {
    "pycsdr.modules.Squelch",
    sizeof(Squelch),
    0,
    Py_TPFLAGS_DEFAULT,
    SquelchSlots
};
