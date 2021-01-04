#include "bandpass_fir_fft_cc.h"

MAKE_WORKER(BandpassFirFft, sizeof(complexf), sizeof(complexf))

int BandpassFirFft_init(BandpassFirFft* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"low_cut", "high_cut", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "ff", kwlist,
                                     &self->low_cut, self->high_cut))
        return -1;

    return 0;
}

PyObject* BandpassFirFft_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    BandpassFirFft* self;
    self = (BandpassFirFft*) type->tp_alloc(type, 0);
    if (self != NULL) {
        Py_INCREF(type);
        WORKER_MEMBER_INIT
        self->low_cut = 0;
        self->high_cut = 0;
    }
    return (PyObject*) self;
}


void* BandpassFirFft_worker(void* ctx) {
    BandpassFirFft* self = (BandpassFirFft*) ctx;

    return NULL;
}

PyObject* BandpassFirFft_setLowCut(BandpassFirFft* self, PyObject* args, PyObject* kwds){
    static char* kwlist[] = {"low_cut", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "f", kwlist,
                                     &self->low_cut))
        return NULL;

    Py_RETURN_NONE;
}

PyObject* BandpassFirFft_setHighCut(BandpassFirFft* self, PyObject* args, PyObject* kwds){
    static char* kwlist[] = {"high_cut", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "f", kwlist,
                                     &self->high_cut))
        return NULL;

    Py_RETURN_NONE;
}

PyMethodDef BandpassFirFft_methods[] = {
    WORKER_METHODS(BandpassFirFft)
    {"setLowCut", (PyCFunction) BandpassFirFft_setLowCut, METH_VARARGS | METH_KEYWORDS,
     "set bandpass low cutoff"
    },
    {"setHighCut", (PyCFunction) BandpassFirFft_setHighCut, METH_VARARGS | METH_KEYWORDS,
     "set bandpass high cutoff"
    },
    {NULL}  /* Sentinel */
};

MAKE_WORKER_TYPE(BandpassFirFft)
