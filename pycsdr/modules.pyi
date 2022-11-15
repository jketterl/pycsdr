from pycsdr.types import Format, AgcProfile

version: str = ...
csdr_version: str = ...


class Writer:
    ...


class Reader:
    def read(self) -> memoryview:
        ...


class Buffer(Writer):
    def __init__(self, format: Format, size: int=None):
        ...

    def getFormat(self) -> Format:
        ...

    def getReader(self) -> BufferReader:
        ...

    def write(self, data: bytes) -> None:
        ...


class BufferReader(Reader):
    def __init__(self, buffer: Buffer):
        ...

    def stop(self) -> None:
        ...

    def resume(self) -> None:
        ...

    def read(self) -> bytes:
        ...


class Source:
    def setWriter(self, writer: Writer) -> None:
        ...

    def getOutputFormat(self) -> Format:
        ...


class Sink:
    def setReader(self, reader: Reader) -> None:
        ...

    def getInputFormat(self) -> Format:
        ...


class Module(Source, Sink):
    def stop(self) -> None:
        ...


class TcpSource(Source):
    def __init__(self, port: int, format: Format):
        ...

    def stop(self) -> None:
        ...


class Fft(Module):
    def __init__(self, size: int, every_n_samples: int):
        ...

    def setEveryNSamples(self, every_n_samples: int) -> None:
        ...


class LogPower(Module):
    def __init__(self, add_db: float = 0.0):
        ...


class LogAveragePower(Module):
    def __init__(self, add_db: float, fft_size: int, avg_number: int):
        ...

    def setAvgNumber(self, avg_number: int) -> None:
        ...


class FftSwap(Module):
    def __init__(self, fft_size: int):
        ...


class FftAdpcm(Module):
    def __init__(self, fft_size: int):
        ...


class FirDecimate(Module):
    def __init__(self, decimation: int, transition: float = 0.05, cutoff: float = 0.5):
        ...


class Bandpass(Module):
    def __init__(self, low_cut: float = 0.0, high_cut: float = 0.0, transition: float = 0.0, use_fft: bool = True):
        ...

    def setBandpass(self, low_cut: float, high_cut: float) -> None:
        ...


class Shift(Module):
    def __init__(self, rate: float = 0.0):
        ...

    def setRate(self, rate: float):
        ...


class Convert(Module):
    def __init__(self, inFormat: Format, outFormat: Format):
        ...


class Squelch(Module):
    def __init__(self, decimation: int, reportInterval: int):
        ...

    def setSquelchLevel(self, level: float) -> None:
        ...

    def setPowerWriter(self, writer: Writer) -> None:
        ...

    def setReportInterval(self, reportInterval: int) -> None:
        ...


class FractionalDecimator(Module):
    def __init__(self, format: Format, decimation: float, numPolyPoints: int = 12, prefilter: bool = False):
        ...


class FmDemod(Module):
    def __init__(self):
        ...


class Limit(Module):
    def __init__(self, maxAmplitude: float = 1.0):
        ...


class NfmDeemphasis(Module):
    def __init__(self, sampleRate: int):
        ...


class WfmDeemphasis(Module):
    def __init__(self, sampleRate: int, tau: float):
        ...


class Agc(Module):
    def __init__(self, format: Format):
        ...

    def setProfile(self, profile: AgcProfile) -> None:
        ...

    def setMaxGain(self, maxGain: float) -> None:
        ...

    def setInitialGain(self, gain: float) -> None:
        ...

    def setReference(self, reference: float) -> None:
        ...


class AmDemod(Module):
    def __init__(self):
        ...


class DcBlock(Module):
    def __init__(self):
        ...


class NoiseFilter(Module):
    def __init__(self, threshold: int, fftSize: int, wndSize: int):
        ...

    def setThreshold(self, threshold: int) -> None:
        ...

    def setWndSize(self, wndSize: int) -> None:
        ...

    def setFftSize(self, fftSize: int) -> None:
        ...


class RealPart(Module):
    def __init__(self):
        ...


class AudioResampler(Module):
    def __init__(self, inputRate: int, outputRate: int):
        ...


class AdpcmEncoder(Module):
    def __init__(self, sync: bool = False):
        ...


class Downmix(Module):
    def __init__(self, channels: int = 2):
        ...


class Gain(Module):
    def __init__(self, format: Format, gain: float):
        ...


class TimingRecovery(Module):
    def __init__(self, decimation: int, loopGain: float, maxError: float, useQ: bool):
        ...


class DBPskDecoder(Module):
    def __init__(self):
        ...


class VaricodeDecoder(Module):
    def __init__(self):
        ...
