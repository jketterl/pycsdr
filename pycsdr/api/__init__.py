from abc import ABC, ABCMeta, abstractmethod


class Buffer(ABC):
    @abstractmethod
    def read(self):
        pass


class Source(ABC):
    @abstractmethod
    def setOutput(self, buffer: Buffer):
        pass


class Sink(ABC):
    @abstractmethod
    def setInput(self, buffer: Buffer):
        pass


class Flow(Source, Sink, metaclass=ABCMeta):
    pass
