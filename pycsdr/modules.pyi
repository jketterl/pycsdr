from pycsdr.types import Format, AgcProfile


class Writer:
    pass


class Reader:
    def read(self) -> bytes:
        pass


class Buffer(Writer):
    def __init__(self, format: Format, size: int=None):
        pass

    def getReader(self) -> BufferReader:
        pass


class BufferReader(Reader):
    def stop(self) -> None:
        pass

    def read(self) -> bytes:
        pass


class Source:
    def setWriter(self, writer: Writer) -> None:
        pass

    def getOutputFormat(self) -> Format:
        pass


class Sink:
    def setReader(self, reader: Reader) -> None:
        pass

    def getInputFormat(self) -> Format:
        pass


class Module(Source, Sink):
    def stop(self) -> None:
        pass


class TcpSource(Source):
    def __init__(self, port: int, format: Format):
        pass

    def stop(self) -> None:
        pass


class Fft(Module):
    def __init__(self, size: int, every_n_samples: int):
        pass

    def setEveryNSamples(self, every_n_samples: int) -> None:
        pass


class LogPower(Module):
    def __init__(self, add_db: float = 0.0):
        pass


class LogAveragePower(Module):
    def __init__(self, add_db: float, fft_size: int, avg_number: int):
        pass

    def setAvgNumber(self, avg_number: int) -> None:
        pass


class FftSwap(Module):
    def __init__(self, fft_size: int):
        pass


class FftAdpcm(Module):
    def __init__(self, fft_size: int):
        pass


class FirDecimate(Module):
    def __init__(self, decimation: int, transition: float = 0.05):
        pass


class Bandpass(Module):
    def __init__(self, low_cut: float = 0.0, high_cut: float = 0.0, transition: float = 0.0, use_fft: bool = True):
        pass

    def setBandpass(self, low_cut: float, high_cut: float) -> None:
        pass


class Shift(Module):
    def __init__(self, rate: float = 0.0):
        pass

    def setRate(self, rate: float):
        pass


class Convert(Module):
    def __init__(self, inFormat: Format, outFormat: Format):
        pass


class Squelch(Module):
    def __init__(self, decimation: int):
        pass

    def setSquelchLevel(self, level: float) -> None:
        pass


class FractionalDecimator(Module):
    def __init__(self, decimation: float, numPolyPoint: int = 12):
        pass


class FmDemod(Module):
    def __init__(self):
        pass


class Limit(Module):
    def __init__(self, maxAmplitude: float = 1.0):
        pass


class NfmDeemphasis(Module):
    def __init__(self, sampleRate: int):
        pass


class Agc(Module):
    def __init__(self, format: Format):
        pass

    def setProfile(self, profile: AgcProfile) -> None:
        pass

    def setMaxGain(self, maxGain: float) -> None:
        pass

    def setInitialGain(self, gain: float) -> None:
        pass

class AmDemod(Module):
    def __init__(self):
        pass


class DcBlock(Module):
    def __init__(self):
        pass


class RealPart(Module):
    def __init__(self):
        pass
