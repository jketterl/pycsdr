#include "agc.h"
#include "types.h"

#include <csdr/agc.hpp>

static int Agc_init(Agc* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "format", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist, FORMAT_TYPE, &self->format)) {
        return -1;
    }

    if (self->format == FORMAT_FLOAT) {
        auto agc = new Csdr::Agc<float>();
        agc->setAttack(0.01);
        agc->setDecay(0.0001);
        agc->setHangTime(600);
        agc->setMaxGain(3.0f);
        self->module = agc;
    } else if (self->format == FORMAT_SHORT) {
        auto agc = new Csdr::Agc<short>();
        agc->setAttack(0.01);
        agc->setDecay(0.0001);
        agc->setHangTime(600);
        agc->setMaxGain(3.0f);
        self->module = agc;
    } else {
        PyErr_SetString(PyExc_ValueError, "unsupported agc format");
        return -1;
    }

    return 0;
}

static PyObject* Agc_setInput(Agc* self, PyObject* args, PyObject* kwds) {
    if (self->format == FORMAT_FLOAT) {
        return Module_setInput<float, float>(self, args, kwds);
    } else if (self->format == FORMAT_SHORT) {
        return Module_setInput<short, short>(self, args, kwds);
    } else {
        PyErr_SetString(PyExc_ValueError, "unsupported agc format");
        return NULL;
    }
}

static PyObject* Agc_setOutput(Agc* self, PyObject* args, PyObject* kwds) {
    if (self->format == FORMAT_FLOAT) {
        return Module_setOutput<float, float>(self, args, kwds);
    } else if (self->format == FORMAT_SHORT) {
        return Module_setOutput<short, short>(self, args, kwds);
    } else {
        PyErr_SetString(PyExc_ValueError, "unsupported agc format");
        return NULL;
    }
}

static PyObject* Agc_getOutputFormat(Agc* self) {
    return self->format;
}

static int Agc_clear(Agc* self) {
    if (self->format == FORMAT_FLOAT) {
        return Module_clear<float>(self);
    } else if (self->format == FORMAT_SHORT) {
        return Module_clear<short>(self);
    } else {
        PyErr_SetString(PyExc_ValueError, "unsupported agc format");
        return -1;
    }
}


static PyMethodDef Agc_methods[] = {
    {"setInput", (PyCFunction) Agc_setInput, METH_VARARGS | METH_KEYWORDS,
     "set the input buffer"
    },
    {"setOutput", (PyCFunction) Agc_setOutput, METH_VARARGS | METH_KEYWORDS,
     "set the output buffer"
    },
    {"getOutputFormat", (PyCFunction) Agc_getOutputFormat, METH_NOARGS,
     "get output format"
    },
    {"stop", (PyCFunction) Module_stop, METH_NOARGS,
     "stop processing"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot AgcSlots[] = {
    {Py_tp_init, (void*) Agc_init},
    {Py_tp_clear, (void*) Agc_clear},
    {Py_tp_methods, Agc_methods},
    {0, 0}
};

PyType_Spec AgcSpec = {
    "pycsdr.modules.Agc",
    sizeof(Agc),
    0,
    Py_TPFLAGS_DEFAULT,
    AgcSlots
};

