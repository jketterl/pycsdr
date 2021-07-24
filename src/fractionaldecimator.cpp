#include "fractionaldecimator.h"
#include "types.h"

#include <csdr/fractionaldecimator.hpp>

static int FractionalDecimator_init(FractionalDecimator* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "format", (char*) "decimation", (char*) "numPolyPoints", NULL};

    PyObject* format;
    float decimation = 0.0f;
    unsigned int numPolyPoints = 12;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!f|I", kwlist, FORMAT_TYPE, &format, &decimation, &numPolyPoints)) {
        return -1;
    }

    if (format == FORMAT_FLOAT) {
        self->setModule(new Csdr::FractionalDecimator<float>(decimation, numPolyPoints));
    } else if (format == FORMAT_COMPLEX_FLOAT) {
        self->setModule(new Csdr::FractionalDecimator<Csdr::complex<float>>(decimation, numPolyPoints));
    } else {
        Py_DECREF(format);
        PyErr_SetString(PyExc_ValueError, "unsupported fractional decimator format");
        return -1;
    }
    self->inputFormat = format;
    self->outputFormat = format;

    return 0;
}

static PyType_Slot FractionalDecimatorSlots[] = {
    {Py_tp_init, (void*) FractionalDecimator_init},
    {0, 0}
};

PyType_Spec FractionalDecimatorSpec = {
    "pycsdr.modules.FractionalDecimator",
    sizeof(FractionalDecimator),
    0,
    Py_TPFLAGS_DEFAULT,
    FractionalDecimatorSlots
};
