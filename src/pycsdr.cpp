#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "buffer.h"
#include "tcpsource.h"

static PyModuleDef pycsdrmodule = {
        PyModuleDef_HEAD_INIT,
        .m_name = "pycsdr",
        .m_doc = "Python bindings for the csdr library",
        .m_size = -1,
};

PyMODINIT_FUNC
PyInit_pycsdr(void) {
    PyObject* BufferType = PyType_FromSpec(&BufferSpec);
    if (BufferType == NULL) return NULL;

    PyObject* TcpSourceType = PyType_FromSpec(&TcpSourceSpec);
    if (TcpSourceType == NULL) return NULL;

    /*
    PyObject* FftType = PyType_FromSpec(&FftSpec);
    if (FftType == NULL) return NULL;

    PyObject* LogPowerType = PyType_FromSpec(&LogPowerSpec);
    if (LogPowerType == NULL) return NULL;

    PyObject* LogAveragePowerType = PyType_FromSpec(&LogAveragePowerSpec);
    if (LogAveragePowerType == NULL) return NULL;

    PyObject* FftExchangeSidesType = PyType_FromSpec(&FftExchangeSidesSpec);
    if (FftExchangeSidesType == NULL) return NULL;

    PyObject* CompressFftAdpcmType = PyType_FromSpec(&CompressFftAdpcmSpec);
    if (CompressFftAdpcmType == NULL) return NULL;

    PyObject* FirDecimateType = PyType_FromSpec(&FirDecimateSpec);
    if (FirDecimateType == NULL) return NULL;

    PyObject* BandpassFirFftType = PyType_FromSpec(&BandpassFirFftSpec);
    if (BandpassFirFftType == NULL) return NULL;
    */

    PyObject *m = PyModule_Create(&pycsdrmodule);
    if (m == NULL)
        return NULL;

    PyModule_AddObject(m, "TcpSource", TcpSourceType);

    PyModule_AddObject(m, "Buffer", BufferType);

    /*
    PyModule_AddObject(m, "Fft", FftType);

    PyModule_AddObject(m, "LogPower", LogPowerType);

    PyModule_AddObject(m, "LogAveragePower", LogAveragePowerType);

    PyModule_AddObject(m, "FftExchangeSides", FftExchangeSidesType);

    PyModule_AddObject(m, "CompressFftAdpcm", CompressFftAdpcmType);

    PyModule_AddObject(m, "FirDecimate", FirDecimateType);

    PyModule_AddObject(m, "BandpassFirFft", BandpassFirFftType);
    */

    return m;
}