#include "squelch.h"
#include "types.h"

#include <csdr/power.hpp>

static int Squelch_init(Squelch* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "decimation", NULL};

    unsigned int decimation = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "I", kwlist, &decimation)) {
        return -1;
    }

    self->inputFormat = FORMAT_COMPLEX_FLOAT;
    self->outputFormat = FORMAT_COMPLEX_FLOAT;
    self->setModule(new Csdr::Squelch(decimation, [] (float level) {
        // TODO do something with the s-meter value
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

static PyMethodDef Squelch_methods[] = {
    {"setSquelchLevel", (PyCFunction) Squelch_setSquelchLevel, METH_VARARGS | METH_KEYWORDS,
     "set squelch level in dB"
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
