#include "shift.hpp"
#include "types.hpp"

#include <csdr/shift.hpp>

static int Shift_init(Shift* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "rate", NULL};

    float rate = 0.0f;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|f", kwlist, &rate)) {
        return -1;
    }

    self->inputFormat = FORMAT_COMPLEX_FLOAT;
    self->outputFormat = FORMAT_COMPLEX_FLOAT;
    self->setModule(new Csdr::ShiftAddfast(rate));

    return 0;
}

static PyObject* Shift_setRate(Shift* self, PyObject* args, PyObject* kwds){
    static char* kwlist[] = {(char*) "rate", NULL};

    float rate = 0.0f;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "f", kwlist, &rate)) {
        return NULL;
    }

    dynamic_cast<Csdr::ShiftAddfast*>(self->module)->setRate(rate);

    Py_RETURN_NONE;
}

static PyMethodDef Shift_methods[] = {
    {"setRate", (PyCFunction) Shift_setRate, METH_VARARGS | METH_KEYWORDS,
     "set shift rate"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot ShiftSlots[] = {
    {Py_tp_init, (void*) Shift_init},
    {Py_tp_methods, Shift_methods},
    {0, 0}
};

PyType_Spec ShiftSpec = {
    "pycsdr.modules.Shift",
    sizeof(Shift),
    0,
    Py_TPFLAGS_DEFAULT,
    ShiftSlots
};
