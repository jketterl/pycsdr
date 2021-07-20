#include "agc.h"
#include "types.h"

#include <csdr/agc.hpp>

static int Agc_init(Agc* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "format", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist, FORMAT_TYPE, &self->format)) {
        return -1;
    }

    if (self->format == FORMAT_FLOAT) {
        self->module = new Csdr::Agc<float>();
    } else if (self->format == FORMAT_SHORT) {
        self->module = new Csdr::Agc<short>();
    } else {
        PyErr_SetString(PyExc_ValueError, "unsupported agc format");
        return -1;
    }

    return 0;
}

static PyObject* Agc_setProfile(Agc* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "profile", NULL};

    PyObject* profile = nullptr;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist, AGC_PROFILE_TYPE, &profile)) {
        return NULL;
    }

    PyObject* attackObj = PyObject_GetAttrString(profile, "attack");
    if (attackObj == NULL) {
        PyErr_SetString(PyExc_AttributeError, "agc profile is missing attack attribute");
        return NULL;
    }
    float attack = PyFloat_AsDouble(attackObj);
    if (PyErr_Occurred()) {
        return NULL;
    }

    PyObject* decayObj = PyObject_GetAttrString(profile, "decay");
    if (decayObj == NULL) {
        PyErr_SetString(PyExc_AttributeError, "agc profile is missing decay attribute");
        return NULL;
    }
    float decay = PyFloat_AsDouble(decayObj);
    if (PyErr_Occurred()) {
        return NULL;
    }

    PyObject* hangTimeObj = PyObject_GetAttrString(profile, "hangTime");
    if (hangTimeObj == NULL) {
        PyErr_SetString(PyExc_AttributeError, "agc profile is missing hangTime attribute");
        return NULL;
    }
    unsigned int hangTime = PyLong_AsUnsignedLong(hangTimeObj);
    if (PyErr_Occurred()) {
        return NULL;
    }

    auto agc = dynamic_cast<Csdr::UntypedAgc*>(self->module);
    agc->setAttack(attack);
    agc->setDecay(decay);
    agc->setHangTime(hangTime);

    Py_RETURN_NONE;
}

static PyObject* Agc_setMaxGain(Agc* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "maxGain", NULL};

    float maxGain = 0.0f;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "f", kwlist, &maxGain)) {
        return NULL;
    }

    dynamic_cast<Csdr::UntypedAgc*>(self->module)->setMaxGain(maxGain);

    Py_RETURN_NONE;
}

static PyObject* Agc_setInitialGain(Agc* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "gain", NULL};

    float gain = 0.0f;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "f", kwlist, &gain)) {
        return NULL;
    }

    dynamic_cast<Csdr::UntypedAgc*>(self->module)->setInitialGain(gain);

    Py_RETURN_NONE;
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
    {"setProfile", (PyCFunction) Agc_setProfile, METH_VARARGS | METH_KEYWORDS,
     "set agc profile"
    },
    {"setMaxGain", (PyCFunction) Agc_setMaxGain, METH_VARARGS | METH_KEYWORDS,
     "set maximum gain"
    },
    {"setInitialGain", (PyCFunction) Agc_setInitialGain, METH_VARARGS | METH_KEYWORDS,
     "set initial / momentary gain value"
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

