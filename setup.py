#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
from setuptools import setup, Extension


version = "0.19.0-dev"

setup(
    name="pycsdr",
    version=version,
    packages=["pycsdr"],

    package_data={
        "pycsdr": ["**.pyi", "**.py"],
    },

    headers=[
        "src/pycsdr.hpp",
        "src/reader.hpp",
        "src/writer.hpp",
        "src/source.hpp",
        "src/sink.hpp",
        "src/module.hpp",
        "src/buffer.hpp",
        "src/bufferreader.hpp",
    ],

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
                "src/wfmdeemphasis.cpp",
                "src/agc.cpp",
                "src/amdemod.cpp",
                "src/dcblock.cpp",
                "src/realpart.cpp",
                "src/audioresampler.cpp",
                "src/adpcmencoder.cpp",
                "src/downmix.cpp",
                "src/gain.cpp",
                "src/timingrecovery.cpp",
                "src/dbpskdecoder.cpp",
                "src/varicodedecoder.cpp",
                "src/phasedemod.cpp",
            ],
            language="c++",
            include_dirs=["src"],
            libraries=['csdr', 'fftw3f'],
            define_macros=[("VERSION", '"{}"'.format(version))],
        )
    ],
)
