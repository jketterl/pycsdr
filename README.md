# PyCSDR

This project implements Python bindings for the [`libcsdr` library of software defined radio components](https://github.com/jketterl/csdr).

It is primarily used for demodulation of radio signals in the OpenWebRX project, however since the csdr components are very generic building blocks for SDR, the classes provided by this project may become useful in other Python SDR applications as well.

# Installation

Please install `libcsdr` and its dependencies before compiling this project.

```
sudo ./setup.py install
```

# TODO

Add an example folder with some generic Python code using this library, e.g. WFM and AM radio demodulators.