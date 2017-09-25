
from setuptools import setup, Extension
from glob import glob
# import os
import sys


# Find source and header files.
sources, headers = ['_irig106.c'], []
sources += glob('libirig106/src/*.c')
headers += glob('libirig106/src/*.h')

# Define flags based on platform.
if sys.platform == 'win32':
    flags = ['/Od', '/EHsc', '/MT']
    link_flags = []
else:
    flags = [
        '-c', '-std=c99',
        '-fPIC',  '-Wall',
        '-D_FILE_OFFSET_BITS=64',
        '-D_LARGEFILE64_SOURCE',
        '-ggdb',
        '-fpack-struct=1',
    ]
    link_flags = ['-fPIC']

_i106 = Extension(
    '_i106',
    sources,
    depends=headers,
    extra_compile_args=flags,
    extra_link_args=link_flags,
)

setup(
    name='libirig106',
    version='0.0.1',
    ext_modules=[_i106],
    packages=['irig106'],
)
