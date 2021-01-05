#include "log_power_cf.h"

MAKE_WORKER(LogPower, sizeof(complexf), sizeof(float))

static void* LogPower_worker(void* ctx) {
    LogPower* self = (LogPower*) ctx;

    uint32_t available;
    while (self->run) {
        available = Buffer_wait(self->inputBuffer, self->read_pos, &self->run);
        if (available == 0) {
            self->run = false;
            break;
        }
        logpower_cf(
            (complexf*) Buffer_getReadPointer(self->inputBuffer, self->read_pos),
            Buffer_getWritePointer_n(self->outputBuffer, available),
            available,
            self->add_db
        );
        Buffer_advanceReadPos(self->inputBuffer, &self->read_pos, available);
        Buffer_advance(self->outputBuffer, available);
    }

    return NULL;
}

static int LogPower_init(LogPower* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"add_db", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "f", kwlist,
                                     &self->add_db))
        return -1;

    return 0;
}

static PyMethodDef LogPower_methods[] = {
    WORKER_METHODS(LogPower)
    {NULL}  /* Sentinel */
};

MAKE_WORKER_SPEC(LogPower)
