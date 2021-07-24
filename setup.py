#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
from setuptools import setup, Extension

try:
    from setuptools import find_namespace_packages
except ImportError:
    from setuptools import PEP420PackageFinder
    find_namespace_packages = PEP420PackageFinder.find


setup(
    name="pycsdr",
    version="0.18.0-dev",
    packages=["pycsdr"],

    package_data={
        "pycsdr": ["**.pyi", "**.py"],
    },

    ext_modules=[
        Extension(
            name="pycsdr.modules",
            sources=[
                "src/pycsdr.cpp",
                "src/writer.cpp",
                "src/reader.cpp",
                "src/sink.cpp",
                "src/source.cpp",
                "src/buffer.cpp",
                "src/bufferreader.cpp",
                "src/tcpsource.cpp",
                "src/types.cpp",
                "src/module.cpp",
                "src/fft.cpp",
                "src/logpower.cpp",
                "src/logaveragepower.cpp",
                "src/fftswap.cpp",
                "src/fftadpcm.cpp",
                "src/firdecimate.cpp",
                "src/bandpass.cpp",
                "src/shift.cpp",
                "src/convert.cpp",
                "src/squelch.cpp",
                "src/fractionaldecimator.cpp",
                "src/fmdemod.cpp",
                "src/limit.cpp",
                "src/nfmdeemphasis.cpp",
                "src/agc.cpp",
                "src/amdemod.cpp",
                "src/dcblock.cpp",
                "src/realpart.cpp",
            ],
            language="c++",
            include_dirs=["src"],
            libraries=['csdr++', 'fftw3f'],
        )
    ],
)
