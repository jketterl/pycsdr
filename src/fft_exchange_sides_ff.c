#include "fft_exchange_sides_ff.h"

MAKE_WORKER(FftExchangeSides, sizeof(float), sizeof(float));

PyObject* FftExchangeSides_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    FftExchangeSides* self;
    self = (FftExchangeSides*) type->tp_alloc(type, 0);
    if (self != NULL) {
        WORKER_MEMBER_INIT
    }
    return (PyObject*) self;
}

void* FftExchangeSides_worker(void* ctx) {
    FftExchangeSides* self = (FftExchangeSides*) ctx;

    uint32_t available;
    while (self->run) {
        uint32_t half = self->fft_size / 2;
        // get a buffer to write to (ensuring minimum space)
        float* output = Buffer_getWritePointer_n(self->outputBuffer, self->fft_size);
        // wait until minimum amount of samples is available on input buffer
        available = Buffer_wait_n(self->inputBuffer, self->read_pos, &self->run, self->fft_size);
        if (available == 0) {
            self->run = false;
            break;
        }
        // switching copy
        memcpy(output, Buffer_getReadPointer(self->inputBuffer, self->read_pos + half), half * sizeof(float));
        memcpy(output + half, Buffer_getReadPointer(self->inputBuffer, self->read_pos), half * sizeof(float));
        // advance the buffers after consuming and producing data
        Buffer_advanceReadPos(self->inputBuffer, &self->read_pos, self->fft_size);
        Buffer_advance(self->outputBuffer, self->fft_size);
    }

    //Buffer_shutdown(self->outputBuffer);

    return NULL;
}

int FftExchangeSides_init(FftExchangeSides* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"fft_size", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "H", kwlist,
                                     &self->fft_size))
        return -1;

    return 0;
}

PyMethodDef FftExchangeSides_methods[] = {
    WORKER_METHODS(FftExchangeSides)
    {NULL}  /* Sentinel */
};

MAKE_WORKER_TYPE(FftExchangeSides)
