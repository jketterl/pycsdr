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
                "src/buffer.cpp",
                "src/tcpsource.cpp",
                "src/types.cpp",
            ],
            language="c++",
            include_dirs=["src"],
            libraries=['csdr++', 'fftw3f'],
        )
    ],
)
