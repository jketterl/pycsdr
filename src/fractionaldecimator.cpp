#include "fractionaldecimator.h"

#include <csdr/fractionaldecimator.hpp>

static int FractionalDecimator_init(FractionalDecimator* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "decimation", (char*) "numPolyPoints", NULL};

    float decimation = 0.0f;
    unsigned int numPolyPoints = 12;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "f|I", kwlist, &decimation, &numPolyPoints)) {
        return -1;
    }

    self->module = new Csdr::FractionalDecimator<float>(decimation, numPolyPoints);

    return 0;
}

static PyMethodDef FractionalDecimator_methods[] = {
    {"setInput", (PyCFunction) Module_setInput<float, float>, METH_VARARGS | METH_KEYWORDS,
     "set the input buffer"
    },
    {"setOutput", (PyCFunction) Module_setOutput<float, float>, METH_VARARGS | METH_KEYWORDS,
     "set the output buffer"
    },
    {"getOutputFormat", (PyCFunction) Module_getOutputFormat<float>, METH_NOARGS,
     "get output format"
    },
    {"stop", (PyCFunction) Module_stop, METH_NOARGS,
     "stop processing"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot FractionalDecimatorSlots[] = {
    {Py_tp_init, (void*) FractionalDecimator_init},
    {Py_tp_clear, (void*) Module_clear<float>},
    {Py_tp_methods, FractionalDecimator_methods},
    {0, 0}
};

PyType_Spec FractionalDecimatorSpec = {
    "pycsdr.modules.FractionalDecimator",
    sizeof(FractionalDecimator),
    0,
    Py_TPFLAGS_DEFAULT,
    FractionalDecimatorSlots
};
