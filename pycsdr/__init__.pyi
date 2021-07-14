class Writer:
    pass

class Reader:
    def read(self):
        pass

class Buffer(Writer, Reader):
    def __init__(self, size=None):
        pass

    def stop(self):
        pass


class Source:
    def setOutput(self, writer: Writer):
        pass

    def stop(self):
        pass


class Sink:
    def setInput(self, reader: Reader):
        pass

    def stop(self):
        pass


class Module(Source, Sink):
    pass


class TcpSource(Source):
    def __init__(self, port: int):
        pass