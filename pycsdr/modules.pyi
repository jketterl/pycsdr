from pycsdr.types import Format


class Writer:
    pass


class Reader:
    def read(self) -> bytes:
        pass


class Buffer(Writer, Reader):
    def __init__(self, format: Format, size: int=None):
        pass

    def stop(self) -> None:
        pass


class Source:
    def setOutput(self, writer: Writer) -> None:
        pass

    def getOutputFormat(self) -> Format:
        pass

    def stop(self) -> None:
        pass


class Sink:
    def setInput(self, reader: Reader) -> None:
        pass

    def stop(self) -> None:
        pass


class Module(Source, Sink):
    pass


class TcpSource(Source):
    def __init__(self, port: int):
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
