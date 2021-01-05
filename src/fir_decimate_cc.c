#include "fir_decimate_cc.h"

MAKE_WORKER(FirDecimate, sizeof(complexf), sizeof(complexf))

static int FirDecimate_init(FirDecimate* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"decimation", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "H", kwlist,
                                     &self->decimation))
        return -1;

    return 0;
}

static void* FirDecimate_worker(void* ctx) {
    FirDecimate* self = (FirDecimate*) ctx;

    return NULL;
}

static PyObject* FirDecimate_setDecimation(FirDecimate* self, PyObject* args, PyObject* kwds){
    static char* kwlist[] = {"decimation", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "H", kwlist,
                                     &self->decimation))
        return NULL;

    Py_RETURN_NONE;
}

static PyMethodDef FirDecimate_methods[] = {
    WORKER_METHODS(FirDecimate)
    {"setDecimation", (PyCFunction) FirDecimate_setDecimation, METH_VARARGS | METH_KEYWORDS,
     "set decimation factor"
    },
    {NULL}  /* Sentinel */
};

MAKE_WORKER_SPEC(FirDecimate)
