#include "logpower.hpp"
#include "types.hpp"

#include <csdr/logpower.hpp>

static int LogPower_init(LogPower* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "add_db", NULL};

    float add_db = 0.0f;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "f", kwlist, &add_db)) {
        return -1;
    }

    self->inputFormat = FORMAT_COMPLEX_FLOAT;
    self->outputFormat = FORMAT_FLOAT;
    self->setModule(new Csdr::LogPower(add_db));

    return 0;
}

static PyType_Slot LogPowerSlots[] = {
    {Py_tp_init, (void*) LogPower_init},
    {0, 0}
};

PyType_Spec LogPowerSpec = {
    "pycsdr.modules.LogPower",
    sizeof(LogPower),
    0,
    Py_TPFLAGS_DEFAULT,
    LogPowerSlots
};
