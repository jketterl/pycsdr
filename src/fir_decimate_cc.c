#include "fir_decimate_cc.h"

MAKE_WORKER(FirDecimate, sizeof(complexf), sizeof(complexf))

static int FirDecimate_init(FirDecimate* self, PyObject* args, PyObject* kwds) {
    // defaults
    self->transition_bw = 0.05;
    self->window = WINDOW_DEFAULT;

    static char* kwlist[] = {"decimation", "transition_bw", "window", NULL};
    static char* window = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "H!fs", kwlist, &self->decimation, &self->transition_bw, &window)) {
        return -1;
    }

    if (window != NULL) {
        self->window = firdes_get_window_from_string(window);
    }

    return 0;
}

static void* FirDecimate_worker(void* ctx) {
    FirDecimate* self = (FirDecimate*) ctx;

    fir_decimate_t decimator = fir_decimate_init(self->decimation, self->transition_bw, self->window);

    uint16_t buffer_size = 1024 * 16;
    while (buffer_size < decimator.taps_length * 2) buffer_size *= 2;
    complexf* input = malloc(sizeof(complexf) * buffer_size);
    complexf* output = malloc(sizeof(complexf) * (buffer_size / self->decimation + 1));

    decimator.write_pointer = input;
    decimator.input_skip = buffer_size;

    int output_size;
    uint32_t read;
    while (self->run) {
        read = Buffer_read_n(self->inputBuffer, decimator.write_pointer, &self->read_pos, &self->run, decimator.input_skip);
        if (read == 0) {
            self->run = false;
            break;
        }
        output_size = fir_decimate_cc(input, output, read, &decimator);
        Buffer_write(self->outputBuffer, output, output_size);
    }

    free(input);
    free(output);

    return NULL;
}

static PyObject* FirDecimate_setDecimation(FirDecimate* self, PyObject* args, PyObject* kwds){
    static char* kwlist[] = {"decimation", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "H", kwlist, &self->decimation)) {
        return NULL;
    }

    // TODO: re-calculate actual decimation

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
