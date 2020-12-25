#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "socket_client.h"
#include "buffer.h"
#include "fft_cc.h"
#include "log_power_cf.h"
#include "log_average_power_cf.h"
#include "fft_exchange_sides_ff.h"
#include "compress_fft_adpcm_f_u8.h"
#include "fir_decimate_cc.h"

static PyModuleDef pycsdrmodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "pycsdr",
    .m_doc = "Python bindings for the csdr library",
    .m_size = -1,
};

PyMODINIT_FUNC
PyInit_pycsdr(void) {
    PyObject* m;
    if (PyType_Ready(&SocketClientType) < 0)
        return NULL;

    if (PyType_Ready(&BufferType) < 0)
        return NULL;

    if (PyType_Ready(&FftType) < 0)
        return NULL;

    if (PyType_Ready(&LogPowerType) < 0)
        return NULL;

    if (PyType_Ready(&LogAveragePowerType) < 0)
        return NULL;

    if (PyType_Ready(&FftExchangeSidesType) < 0)
        return NULL;

    if (PyType_Ready(&CompressFftAdpcmType) < 0)
        return NULL;

    if (PyType_Ready(&FirDecimateType) < 0)
        return NULL;

    m = PyModule_Create(&pycsdrmodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&SocketClientType);
    PyModule_AddObject(m, "SocketClient", (PyObject*) &SocketClientType);

    Py_INCREF(&BufferType);
    PyModule_AddObject(m, "Buffer", (PyObject*) &BufferType);

    Py_INCREF(&FftType);
    PyModule_AddObject(m, "Fft", (PyObject*) &FftType);

    Py_INCREF(&LogPowerType);
    PyModule_AddObject(m, "LogPower", (PyObject*) &LogPowerType);

    Py_INCREF(&LogAveragePowerType);
    PyModule_AddObject(m, "LogAveragePower", (PyObject*) &LogAveragePowerType);

    Py_INCREF(&FftExchangeSidesType);
    PyModule_AddObject(m, "FftExchangeSides", (PyObject*) &FftExchangeSidesType);

    Py_INCREF(&CompressFftAdpcmType);
    PyModule_AddObject(m, "CompressFftAdpcm", (PyObject*) &CompressFftAdpcmType);

    Py_INCREF(&FirDecimateType);
    PyModule_AddObject(m, "FirDecimate", (PyObject*) &FirDecimateType);

    return m;
}
