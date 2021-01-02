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
    packages=find_namespace_packages(include=["pycsdr.api"]),

    package_data={
        "": ["**.txt", "**.md", "**.py", "**.h", "**.hpp", "**.c", "**.cpp"],
    },

    ext_modules=[
        Extension(
            name="pycsdr.modules",
            sources=[
                "src/pycsdr_module.c",
                "src/worker_template.c",
                "src/socket_client.c",
                "src/buffer.c",
                "src/fft_cc.c",
                "src/log_power_cf.c",
                "src/log_average_power_cf.c",
                "src/fft_exchange_sides_ff.c",
                "src/compress_fft_adpcm_f_u8.c",
                "src/fir_decimate_cc.c",
                "src/bandpass_fir_fft_cc.c",
            ],
            include_dirs=["src"],
            libraries=['csdr', 'fftw3f'],
            define_macros=[("USE_IMA_ADPCM", None)],
        )
    ],
)
