#include "log_average_power_cf.h"

MAKE_WORKER(LogAveragePower, sizeof(complexf), sizeof(float))

static void* LogAveragePower_worker(void* ctx) {
    LogAveragePower* self = (LogAveragePower*) ctx;

    float add_db = self->add_db - 10.0 * log10(self->avg_number);
    uint32_t available;
    while (self->run) {
        // get an area to write to from the output buffer
        float* output = Buffer_getWritePointer_n(self->outputBuffer, self->fft_size);
        // zero it
        memset(output, 0, sizeof(float) * self->fft_size);
        for (int i = 0; i < self->avg_number; i++) {
            // wait for input samples
            available = Buffer_wait_n(self->inputBuffer, self->read_pos, &self->run, self->fft_size);
            if (available == 0) {
                self->run = false;
                break;
            }
            accumulate_power_cf((complexf*) Buffer_getReadPointer(self->inputBuffer, self->read_pos), output, self->fft_size);
            Buffer_advanceReadPos(self->inputBuffer, &self->read_pos, self->fft_size);
        }
        if (available == 0) {
            self->run = false;
            break;
        }
        log_ff(output, output, self->fft_size, add_db);
        // inform output buffer that data is ready
        Buffer_advance(self->outputBuffer, self->fft_size);
    }

    return NULL;
}

static int LogAveragePower_init(LogAveragePower* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"add_db", "fft_size", "avg_number", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "fHH", kwlist,
                                     &self->add_db, &self->fft_size, &self->avg_number))
        return -1;

    return 0;
}

static PyObject* LogAveragePower_setFftAverages(LogAveragePower* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"avg_number", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "H", kwlist,
                                     &self->avg_number))
        return NULL;

    Py_RETURN_NONE;
}

static PyMethodDef LogAveragePower_methods[] = {
    WORKER_METHODS(LogAveragePower)
    {"setFftAverages", (PyCFunction) LogAveragePower_setFftAverages, METH_VARARGS | METH_KEYWORDS,
     "set fft averageing factor"
    },
    {NULL}  /* Sentinel */
};

MAKE_WORKER_SPEC(LogAveragePower)
