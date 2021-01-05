#include "compress_fft_adpcm_f_u8.h"

MAKE_WORKER(CompressFftAdpcm, sizeof(float), sizeof(unsigned char))

static int CompressFftAdpcm_init(CompressFftAdpcm* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"fft_size", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "H", kwlist,
                                     &self->fft_size))
        return -1;

    return 0;
}

static void* CompressFftAdpcm_worker(void* ctx) {
    CompressFftAdpcm* self = (CompressFftAdpcm*) ctx;

    fft_compress_ima_adpcm_t job;
    fft_compress_ima_adpcm_init(&job, self->fft_size);

    uint32_t read;
    while (self->run) {
        read = Buffer_read_n(self->inputBuffer, fft_compress_ima_adpcm_get_write_pointer(&job), &self->read_pos, &self->run, self->fft_size);
        if (read == 0) {
            self->run = false;
            break;
        }
        fft_compress_ima_adpcm(&job, Buffer_getWritePointer_n(self->outputBuffer, job.real_data_size / 2));
        Buffer_advance(self->outputBuffer, job.real_data_size / 2);
    }

    fft_compress_ima_adpcm_free(&job);

    return NULL;
}

static PyMethodDef CompressFftAdpcm_methods[] = {
    WORKER_METHODS(CompressFftAdpcm)
    {NULL}  /* Sentinel */
};

MAKE_WORKER_SPEC(CompressFftAdpcm)
