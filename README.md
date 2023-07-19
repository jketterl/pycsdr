# PyCSDR

This project implements Python bindings for the [`libcsdr` library of software defined radio components](https://github.com/jketterl/csdr).

It is primarily used for demodulation of radio signals in the OpenWebRX project, however since the csdr components are very generic building blocks for SDR, the classes provided by this project may become useful in other Python SDR applications as well.

# Installation

The OpenWebRX project is hosting pycsdr packages in their repositories. Please click the respective link for [Debian](https://www.openwebrx.de/download/debian.php) or [Ubuntu](https://www.openwebrx.de/download/ubuntu.php). Due to naming conventions, the repository package is called `python3-csdr`.

# Compiling from source

Please install `libcsdr` and its dependencies before compiling this project.

Please also install the python development files (`libpython3-dev` on most Debian-based distributions).

```
sudo ./setup.py install install_headers
```

# TODO

Add an example folder with some generic Python code using this library, e.g. WFM and AM radio demodulators.