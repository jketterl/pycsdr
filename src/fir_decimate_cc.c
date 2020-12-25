#include "fir_decimate_cc.h"

MAKE_WORKER(FirDecimate, sizeof(complexf), sizeof(complexf))

int FirDecimate_init(FirDecimate* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"decimation", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "H", kwlist,
                                     &self->decimation))
        return -1;

    return 0;
}

PyObject* FirDecimate_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    FirDecimate* self;
    self = (FirDecimate*) type->tp_alloc(type, 0);
    if (self != NULL) {
        WORKER_MEMBER_INIT
    }
    return (PyObject*) self;
}


void* FirDecimate_worker(void* ctx) {
    FirDecimate* self = (FirDecimate*) ctx;

    return NULL;
}

PyObject* FirDecimate_setDecimation(FirDecimate* self, PyObject* args, PyObject* kwds){
    static char* kwlist[] = {"decimation", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "H", kwlist,
                                     &self->decimation))
        return NULL;

    Py_RETURN_NONE;
}

PyMethodDef FirDecimate_methods[] = {
    WORKER_METHODS(FirDecimate)
    {"setDecimation", (PyCFunction) FirDecimate_setDecimation, METH_VARARGS | METH_KEYWORDS,
     "set decimation factor"
    },
    {NULL}  /* Sentinel */
};

MAKE_WORKER_TYPE(FirDecimate)
