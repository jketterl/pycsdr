#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "buffer.h"
#include "tcpsource.h"
#include "fft.h"
#include "logpower.h"
#include "logaveragepower.h"
#include "fftswap.h"
#include "fftadpcm.h"
#include "firdecimate.h"

static PyModuleDef pycsdrmodule = {
        PyModuleDef_HEAD_INIT,
        .m_name = "pycsdr.modules",
        .m_doc = "Python bindings for the csdr library",
        .m_size = -1,
};

PyMODINIT_FUNC
PyInit_modules(void) {
    PyObject* BufferType = PyType_FromSpec(&BufferSpec);
    if (BufferType == NULL) return NULL;

    PyObject* TcpSourceType = PyType_FromSpec(&TcpSourceSpec);
    if (TcpSourceType == NULL) return NULL;

    PyObject* FftType = PyType_FromSpec(&FftSpec);
    if (FftType == NULL) return NULL;

    PyObject* LogPowerType = PyType_FromSpec(&LogPowerSpec);
    if (LogPowerType == NULL) return NULL;

    PyObject* LogAveragePowerType = PyType_FromSpec(&LogAveragePowerSpec);
    if (LogAveragePowerType == NULL) return NULL;

    PyObject* FftSwapType = PyType_FromSpec(&FftSwapSpec);
    if (FftSwapType == NULL) return NULL;

    PyObject* FftAdpcmType = PyType_FromSpec(&FftAdpcmSpec);
    if (FftAdpcmType == NULL) return NULL;

    PyObject* FirDecimateType = PyType_FromSpec(&FirDecimateSpec);
    if (FirDecimateType == NULL) return NULL;

    /*
    PyObject* BandpassFirFftType = PyType_FromSpec(&BandpassFirFftSpec);
    if (BandpassFirFftType == NULL) return NULL;
    */

    PyObject *m = PyModule_Create(&pycsdrmodule);
    if (m == NULL) {
        return NULL;
    }

    PyModule_AddObject(m, "TcpSource", TcpSourceType);

    PyModule_AddObject(m, "Buffer", BufferType);

    PyModule_AddObject(m, "Fft", FftType);

    PyModule_AddObject(m, "LogPower", LogPowerType);

    PyModule_AddObject(m, "LogAveragePower", LogAveragePowerType);

    PyModule_AddObject(m, "FftSwap", FftSwapType);

    PyModule_AddObject(m, "FftAdpcm", FftAdpcmType);

    PyModule_AddObject(m, "FirDecimate", FirDecimateType);

    /*
    PyModule_AddObject(m, "BandpassFirFft", BandpassFirFftType);
    */

    return m;
}