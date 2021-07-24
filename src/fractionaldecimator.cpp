#include "fractionaldecimator.h"
#include "types.h"

#include <csdr/fractionaldecimator.hpp>

static int FractionalDecimator_init(FractionalDecimator* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "decimation", (char*) "numPolyPoints", NULL};

    float decimation = 0.0f;
    unsigned int numPolyPoints = 12;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "f|I", kwlist, &decimation, &numPolyPoints)) {
        return -1;
    }

    self->inputFormat = FORMAT_FLOAT;
    self->outputFormat = FORMAT_FLOAT;
    self->setModule(new Csdr::FractionalDecimator<float>(decimation, numPolyPoints));

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
