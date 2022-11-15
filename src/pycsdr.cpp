#include "pycsdr.hpp"
#include "buffer.hpp"
#include "tcpsource.hpp"
#include "fft.hpp"
#include "logpower.hpp"
#include "logaveragepower.hpp"
#include "fftswap.hpp"
#include "fftadpcm.hpp"
#include "firdecimate.hpp"
#include "bandpass.hpp"
#include "shift.hpp"
#include "squelch.hpp"
#include "fractionaldecimator.hpp"
#include "fmdemod.hpp"
#include "limit.hpp"
#include "nfmdeemphasis.hpp"
#include "wfmdeemphasis.hpp"
#include "agc.hpp"
#include "convert.hpp"
#include "amdemod.hpp"
#include "dcblock.hpp"
#include "realpart.hpp"
#include "writer.hpp"
#include "reader.hpp"
#include "sink.hpp"
#include "source.hpp"
#include "bufferreader.hpp"
#include "module.hpp"
#include "audioresampler.hpp"
#include "adpcmencoder.hpp"
#include "downmix.hpp"
#include "gain.hpp"
#include "timingrecovery.hpp"
#include "dbpskdecoder.hpp"
#include "varicodedecoder.hpp"
#include "noisefilter.hpp"

#include <csdr/version.hpp>

static PyModuleDef pycsdrmodule = {
        PyModuleDef_HEAD_INIT,
        .m_name = "pycsdr.modules",
        .m_doc = "Python bindings for the csdr library",
        .m_size = -1,
};

PyTypeObject* WriterType;

PyTypeObject* ReaderType;

PyTypeObject* SinkType;

PyTypeObject* SourceType;

PyTypeObject* ModuleType;

PyTypeObject* BufferType;

PyTypeObject* BufferReaderType;

PyMODINIT_FUNC
PyInit_modules(void) {
    WriterType = (PyTypeObject*) PyType_FromSpec(&WriterSpec);
    if (WriterType == NULL) return NULL;

    ReaderType = (PyTypeObject*) PyType_FromSpec(&ReaderSpec);
    if (ReaderType == NULL) return NULL;

    SinkType = (PyTypeObject*) PyType_FromSpec(&SinkSpec);
    if (SinkType == NULL) return NULL;

    SourceType = (PyTypeObject*) PyType_FromSpec(&SourceSpec);
    if (SourceType == NULL) return NULL;

    /*
    Py_INCREF(SinkType);
    Py_INCREF(SourceType);
    PyObject* bases = PyTuple_Pack(2, SinkType, SourceType);
    if (bases == NULL) return NULL;
    */
    ModuleType = (PyTypeObject*) PyType_FromSpec/*WithBases*/(&ModuleSpec/*, bases*/);
    if (ModuleType == NULL) return NULL;

    Py_INCREF(WriterType);
    PyObject* bases = PyTuple_Pack(1, WriterType);
    if (bases == NULL) return NULL;
    BufferType = (PyTypeObject*) PyType_FromSpecWithBases(&BufferSpec, bases);
    if (BufferType == NULL) return NULL;

    Py_INCREF(ReaderType);
    bases = PyTuple_Pack(1, ReaderType);
    if (bases == NULL) return NULL;
    BufferReaderType = (PyTypeObject*) PyType_FromSpecWithBases(&BufferReaderSpec, bases);
    if (BufferReaderType == NULL) return NULL;

    Py_INCREF(SourceType);
    bases = PyTuple_Pack(1, SourceType);
    if (bases == NULL) return NULL;
    PyObject* TcpSourceType = PyType_FromSpecWithBases(&TcpSourceSpec, bases);
    if (TcpSourceType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* FftType = PyType_FromSpecWithBases(&FftSpec, bases);
    if (FftType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* LogPowerType = PyType_FromSpecWithBases(&LogPowerSpec, bases);
    if (LogPowerType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* LogAveragePowerType = PyType_FromSpecWithBases(&LogAveragePowerSpec, bases);
    if (LogAveragePowerType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* FftSwapType = PyType_FromSpecWithBases(&FftSwapSpec, bases);
    if (FftSwapType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* FftAdpcmType = PyType_FromSpecWithBases(&FftAdpcmSpec, bases);
    if (FftAdpcmType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* FirDecimateType = PyType_FromSpecWithBases(&FirDecimateSpec, bases);
    if (FirDecimateType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* BandpassType = PyType_FromSpecWithBases(&BandpassSpec, bases);
    if (BandpassType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* ShiftType = PyType_FromSpecWithBases(&ShiftSpec, bases);
    if (ShiftType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* SquelchType = PyType_FromSpecWithBases(&SquelchSpec, bases);
    if (SquelchType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* FractionalDecimatorType = PyType_FromSpecWithBases(&FractionalDecimatorSpec, bases);
    if (FractionalDecimatorType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* FmDemodType = PyType_FromSpecWithBases(&FmDemodSpec, bases);
    if (FmDemodType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* LimitType = PyType_FromSpecWithBases(&LimitSpec, bases);
    if (LimitType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* NfmDeemphasisType = PyType_FromSpecWithBases(&NfmDeemphasisSpec, bases);
    if (NfmDeemphasisType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* WfmDeemphasisType = PyType_FromSpecWithBases(&WfmDeemphasisSpec, bases);
    if (WfmDeemphasisType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* AgcType = PyType_FromSpecWithBases(&AgcSpec, bases);
    if (AgcType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* ConvertType = PyType_FromSpecWithBases(&ConvertSpec, bases);
    if (ConvertType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* AmDemodType = PyType_FromSpecWithBases(&AmDemodSpec, bases);
    if (AmDemodType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* DcBlockType = PyType_FromSpecWithBases(&DcBlockSpec, bases);
    if (DcBlockType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* NoiseFilterType = PyType_FromSpecWithBases(&NoiseFilterSpec, bases);
    if (NoiseFilterType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* RealPartType = PyType_FromSpecWithBases(&RealPartSpec, bases);
    if (RealPartType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* AudioResamplerType = PyType_FromSpecWithBases(&AudioResamplerSpec, bases);
    if (AudioResamplerType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* AdpcmEncoderType = PyType_FromSpecWithBases(&AdpcmEncoderSpec, bases);
    if (AdpcmEncoderType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* DownmixType = PyType_FromSpecWithBases(&DownmixSpec, bases);
    if (DownmixType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* GainType = PyType_FromSpecWithBases(&GainSpec, bases);
    if (GainType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* TimingRecoveryType = PyType_FromSpecWithBases(&TimingRecoverySpec, bases);
    if (TimingRecoveryType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* DBPskDecoderType = PyType_FromSpecWithBases(&DBPskDecoderSpec, bases);
    if (DBPskDecoderType == NULL) return NULL;

    Py_INCREF(ModuleType);
    bases = PyTuple_Pack(1, ModuleType);
    if (bases == NULL) return NULL;
    PyObject* VaricodeDecoderType = PyType_FromSpecWithBases(&VaricodeDecoderSpec, bases);
    if (VaricodeDecoderType == NULL) return NULL;

    PyObject *m = PyModule_Create(&pycsdrmodule);
    if (m == NULL) {
        return NULL;
    }

    PyModule_AddObject(m, "Reader", (PyObject*) ReaderType);

    PyModule_AddObject(m, "Writer", (PyObject*) WriterType);

    PyModule_AddObject(m, "Sink", (PyObject*) SinkType);

    PyModule_AddObject(m, "Source", (PyObject*) SourceType);

    PyModule_AddObject(m, "Module", (PyObject*) ModuleType);

    PyModule_AddObject(m, "TcpSource", TcpSourceType);

    PyModule_AddObject(m, "Buffer", (PyObject*) BufferType);

    PyModule_AddObject(m, "BufferReader", (PyObject*) BufferReaderType);

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

    PyModule_AddObject(m, "WfmDeemphasis", WfmDeemphasisType);

    PyModule_AddObject(m, "Agc", AgcType);

    PyModule_AddObject(m, "Convert", ConvertType);

    PyModule_AddObject(m, "AmDemod", AmDemodType);

    PyModule_AddObject(m, "DcBlock", DcBlockType);

    PyModule_AddObject(m, "NoiseFilter", NoiseFilterType);

    PyModule_AddObject(m, "RealPart", RealPartType);

    PyModule_AddObject(m, "AudioResampler", AudioResamplerType);

    PyModule_AddObject(m, "AdpcmEncoder", AdpcmEncoderType);

    PyModule_AddObject(m, "Downmix", DownmixType);

    PyModule_AddObject(m, "Gain", GainType);

    PyModule_AddObject(m, "TimingRecovery", TimingRecoveryType);

    PyModule_AddObject(m, "DBPskDecoder", DBPskDecoderType);

    PyModule_AddObject(m, "VaricodeDecoder", VaricodeDecoderType);

    PyObject* csdrVersion = PyUnicode_FromStringAndSize(Csdr::version.c_str(), Csdr::version.length());
    if (csdrVersion == NULL) return NULL;
    PyModule_AddObject(m, "csdr_version", csdrVersion);

    PyObject* version = PyUnicode_FromString(VERSION);
    if (version == NULL) return NULL;
    PyModule_AddObject(m, "version", version);

    return m;
}