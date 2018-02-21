#!/usr/bin/env python

from distutils.cmd import Command
from glob import glob
from setuptools import setup, Extension
from shutil import rmtree
import os
import sys


class Clean(Command):
    description = 'clean build and dist directories'
    user_options = []

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        for path in ('build', 'dist', 'i106.egg-info'):
            if os.path.exists(path):
                rmtree(path)


# Find source and header files.
sources, headers = [], []
sources += glob('src/*.c')
headers += glob('src/*.h')
sources += glob('src/libirig106/src/*.c')
headers += glob('src/libirig106/src/*.h')

# Define flags based on platform.
link_flags = []
if sys.platform == 'win32':
    flags = ['/Od', '/EHsc', '/MT']
else:
    flags = [
        '-c',
        '-std=c99',
        '-fPIC',
        '-Wall',
        '-D_FILE_OFFSET_BITS=64',
        '-D_LARGEFILE64_SOURCE',
        '-ggdb',
    ]
    link_flags = ['-fPIC']

ext = Extension(
    'i106',
    sources,
    depends=headers,
    extra_compile_args=flags,
    extra_link_args=link_flags,
)

setup(
    name='i106',
    version='0.0.1',
    ext_modules=[ext],
    cmdclass={'clean': Clean},
    setup_requires=['pytest-runner'],
    tests_require=['pytest'],
)
