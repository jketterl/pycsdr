#include "agc.hpp"
#include "types.hpp"

#include <csdr/agc.hpp>

static int Agc_init(Agc* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "format", NULL};

    PyObject* format;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist, FORMAT_TYPE, &format)) {
        return -1;
    }

    if (format == FORMAT_FLOAT) {
        self->setModule(new Csdr::Agc<float>());
    } else if (format == FORMAT_SHORT) {
        self->setModule(new Csdr::Agc<short>());
    } else if (format == FORMAT_COMPLEX_FLOAT) {
        self->setModule(new Csdr::Agc<Csdr::complex<float>>());
    } else {
        PyErr_SetString(PyExc_ValueError, "unsupported agc format");
        return -1;
    }

    Py_INCREF(format);
    self->inputFormat = format;
    Py_INCREF(format);
    self->outputFormat = format;

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

static PyObject* Agc_setReference(Agc* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "reference", NULL};

    float reference = 0.0f;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "f", kwlist, &reference)) {
        return NULL;
    }

    dynamic_cast<Csdr::UntypedAgc*>(self->module)->setReference(reference);

    Py_RETURN_NONE;
}

static PyMethodDef Agc_methods[] = {
    {"setProfile", (PyCFunction) Agc_setProfile, METH_VARARGS | METH_KEYWORDS,
     "set agc profile"
    },
    {"setMaxGain", (PyCFunction) Agc_setMaxGain, METH_VARARGS | METH_KEYWORDS,
     "set maximum gain"
    },
    {"setInitialGain", (PyCFunction) Agc_setInitialGain, METH_VARARGS | METH_KEYWORDS,
     "set initial / momentary gain value"
    },
    {"setReference", (PyCFunction) Agc_setReference, METH_VARARGS | METH_KEYWORDS,
     "set agc reference level"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot AgcSlots[] = {
    {Py_tp_init, (void*) Agc_init},
    {Py_tp_methods, Agc_methods},
    {0, 0}
};

PyType_Spec AgcSpec = {
    "pycsdr.modules.Agc",
    sizeof(Agc),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    AgcSlots
};

