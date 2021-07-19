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
#include "bandpass.h"
#include "shift.h"
#include "squelch.h"
#include "fractionaldecimator.h"
#include "fmdemod.h"
#include "limit.h"
#include "nfmdeemphasis.h"
#include "agc.h"
#include "convert.h"
#include "amdemod.h"
#include "dcblock.h"
#include "realpart.h"

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

    PyObject* BandpassType = PyType_FromSpec(&BandpassSpec);
    if (BandpassType == NULL) return NULL;

    PyObject* ShiftType = PyType_FromSpec(&ShiftSpec);
    if (ShiftType == NULL) return NULL;

    PyObject* SquelchType = PyType_FromSpec(&SquelchSpec);
    if (SquelchType == NULL) return NULL;

    PyObject* FractionalDecimatorType = PyType_FromSpec(&FractionalDecimatorSpec);
    if (FractionalDecimatorType == NULL) return NULL;

    PyObject* FmDemodType = PyType_FromSpec(&FmDemodSpec);
    if (FmDemodType == NULL) return NULL;

    PyObject* LimitType = PyType_FromSpec(&LimitSpec);
    if (LimitType == NULL) return NULL;

    PyObject* NfmDeemphasisType = PyType_FromSpec(&NfmDeemphasisSpec);
    if (NfmDeemphasisType == NULL) return NULL;

    PyObject* AgcType = PyType_FromSpec(&AgcSpec);
    if (AgcType == NULL) return NULL;

    PyObject* ConvertType = PyType_FromSpec(&ConvertSpec);
    if (ConvertType == NULL) return NULL;

    PyObject* AmDemodType = PyType_FromSpec(&AmDemodSpec);
    if (AmDemodType == NULL) return NULL;

    PyObject* DcBlockType = PyType_FromSpec(&DcBlockSpec);
    if (DcBlockType == NULL) return NULL;

    PyObject* RealPartType = PyType_FromSpec(&RealPartSpec);
    if (RealPartType == NULL) return NULL;

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

    PyModule_AddObject(m, "Bandpass", BandpassType);

    PyModule_AddObject(m, "Shift", ShiftType);

    PyModule_AddObject(m, "Squelch", SquelchType);

    PyModule_AddObject(m, "FractionalDecimator", FractionalDecimatorType);

    PyModule_AddObject(m, "FmDemod", FmDemodType);

    PyModule_AddObject(m, "Limit", LimitType);

    PyModule_AddObject(m, "NfmDeemphasis", NfmDeemphasisType);

    PyModule_AddObject(m, "Agc", AgcType);

    PyModule_AddObject(m, "Convert", ConvertType);

    PyModule_AddObject(m, "AmDemod", AmDemodType);

    PyModule_AddObject(m, "DcBlock", DcBlockType);

    PyModule_AddObject(m, "RealPart", RealPartType);

    return m;
}