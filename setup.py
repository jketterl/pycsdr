#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
from setuptools import setup, Extension

setup(
    name="pycsdr",
    version="0.18.0-dev",

    package_data={
        "": ["**.txt", "**.md", "**.py", "**.h", "**.hpp", "**.c", "**.cpp"],
    },

    ext_modules=[
        Extension(
            name="pycsdr",
            sources=[
                "src/pycsdr_module.c",
                "src/socket_client.c",
                "src/buffer.c",
                "src/fft_cc.c",
                "src/log_power_cf.c",
                "src/log_average_power_cf.c",
                "src/fft_exchange_sides_ff.c",
                "src/compress_fft_adpcm_f_u8.c",
            ],
            include_dirs=["src"],
            libraries=['csdr', 'fftw3f'],
            define_macros=[("USE_IMA_ADPCM", None)],
        )
    ],
)
