#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "api.h"
#include "socket_client.h"
#include "buffer.h"
#include "fft_cc.h"
#include "log_power_cf.h"
#include "log_average_power_cf.h"
#include "fft_exchange_sides_ff.h"
#include "compress_fft_adpcm_f_u8.h"
#include "fir_decimate_cc.h"
#include "bandpass_fir_fft_cc.h"

static PyModuleDef pycsdrmodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "pycsdr.modules",
    .m_doc = "Python bindings for the csdr library",
    .m_size = -1,
};

PyMODINIT_FUNC
PyInit_modules(void) {
    PyObject* api_SourceBases = getApiTypeTuple("Source");
    if (api_SourceBases == NULL) return NULL;

    PyObject* api_BufferBases = getApiTypeTuple("Buffer");
    if (api_BufferBases == NULL) return NULL;

    PyObject* api_FlowBases = getApiTypeTuple("Flow");
    if (api_FlowBases == NULL) return NULL;

    PyObject* SocketClientType = PyType_FromSpecWithBases(&SocketClientSpec, api_SourceBases);
    if (SocketClientType == NULL) return NULL;

    PyObject* BufferType = PyType_FromSpecWithBases(&BufferSpec, api_BufferBases);
    if (BufferType == NULL) return NULL;

    PyObject* FftType = PyType_FromSpecWithBases(&FftSpec, api_FlowBases);
    if (FftType == NULL) return NULL;

    PyObject* LogPowerType = PyType_FromSpecWithBases(&LogPowerSpec, api_FlowBases);
    if (LogPowerType == NULL) return NULL;

    PyObject* LogAveragePowerType = PyType_FromSpecWithBases(&LogAveragePowerSpec, api_FlowBases);
    if (LogAveragePowerType == NULL) return NULL;

    PyObject* FftExchangeSidesType = PyType_FromSpecWithBases(&FftExchangeSidesSpec, api_FlowBases);
    if (FftExchangeSidesType == NULL) return NULL;

    PyObject* CompressFftAdpcmType = PyType_FromSpecWithBases(&CompressFftAdpcmSpec, api_FlowBases);
    if (CompressFftAdpcmType == NULL) return NULL;

    PyObject* FirDecimateType = PyType_FromSpecWithBases(&FirDecimateSpec, api_FlowBases);
    if (FirDecimateType == NULL) return NULL;

    PyObject* BandpassFirFftType = PyType_FromSpecWithBases(&BandpassFirFftSpec, api_FlowBases);
    if (BandpassFirFftType == NULL) return NULL;

    PyObject* m = PyModule_Create(&pycsdrmodule);
    if (m == NULL)
        return NULL;

    PyModule_AddObject(m, "SocketClient", SocketClientType);

    PyModule_AddObject(m, "Buffer", BufferType);

    PyModule_AddObject(m, "Fft", FftType);

    PyModule_AddObject(m, "LogPower", LogPowerType);

    PyModule_AddObject(m, "LogAveragePower", LogAveragePowerType);

    PyModule_AddObject(m, "FftExchangeSides", FftExchangeSidesType);

    PyModule_AddObject(m, "CompressFftAdpcm", CompressFftAdpcmType);

    PyModule_AddObject(m, "FirDecimate", FirDecimateType);

    PyModule_AddObject(m, "BandpassFirFft", BandpassFirFftType);

    Py_DECREF(api_SourceBases);
    Py_DECREF(api_BufferBases);
    Py_DECREF(api_FlowBases);

    return m;
}
