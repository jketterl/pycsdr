#include "fft_cc.h"

MAKE_WORKER(Fft, sizeof(complexf), sizeof(complexf))

static int Fft_init(Fft* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"size", "every_n_samples", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "HH", kwlist,
                                     &self->size, &self->every_n_samples))
        return -1;

    if (log2n(self->size) == -1) {
        PyErr_SetString(PyExc_ValueError, "fft_size should be power of 2");
        return -1;
    }

    return 0;
}

static void* Fft_worker(void* ctx) {
    Fft* self = (Fft*) ctx;

    window_t window = WINDOW_DEFAULT;
    complexf* input = (complexf*) fftwf_malloc(sizeof(complexf) * self->size);
    complexf* windowed = (complexf*) fftwf_malloc(sizeof(complexf) * self->size);
    complexf* output = (complexf*) fftwf_malloc(sizeof(complexf) * self->size);

	fftwf_plan plan = fftwf_plan_dft_1d(self->size, (fftwf_complex*) windowed, (fftwf_complex*) output, FFTW_FORWARD, FFTW_ESTIMATE);

	float* windowt = precalculate_window(self->size, window);

	uint32_t read;

    while (self->run) {
        if (self->every_n_samples > self->size) {
            read = Buffer_read_n(self->inputBuffer, input, &self->read_pos, &self->run, self->size);
            if (read == 0) {
                self->run = false;
                break;
            }
            //skipping samples before next FFT
            read = Buffer_skip_n(self->inputBuffer, &self->read_pos, &self->run, self->every_n_samples - self->size);
            if (read == 0) {
                self->run = false;
                break;
            }
        } else {
            //overlapped FFT
            for (int i = 0; i < self->size - self->every_n_samples; i++) input[i] = input[i + self->every_n_samples];
            read = Buffer_read_n(self->inputBuffer, input + self->size - self->every_n_samples, &self->read_pos, &self->run, self->every_n_samples);
            if (read == 0) {
                self->run = false;
                break;
            }
        }
        apply_precalculated_window_c(input, windowed, self->size, windowt);
        fftwf_execute(plan);
        Buffer_write(self->outputBuffer, output, self->size);
    }

    return NULL;
}

static PyObject* Fft_setEveryNSamples(Fft* self, PyObject* args, PyObject* kwds){
    static char* kwlist[] = {"every_n_samples", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "H", kwlist,
                                     &self->every_n_samples))
        return NULL;

    Py_RETURN_NONE;
}

static PyMethodDef Fft_methods[] = {
    WORKER_METHODS(Fft)
    {"setEveryNSamples", (PyCFunction) Fft_setEveryNSamples, METH_VARARGS | METH_KEYWORDS,
     "set repetition interval in samples"
    },
    {NULL}  /* Sentinel */
};

MAKE_WORKER_SPEC(Fft)
