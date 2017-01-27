
from setuptools import setup, Extension
import os
import sys


# Find source and header files.
sources, headers = ['_irig106.c'], []
for pth, _, filenames in os.walk('libirig106/src'):
    for filename in filenames:
        filename = os.path.join(pth, filename)
        if filename.endswith(('.c')):

            # Skip broken c files for now.
            if 'analog' in filename.lower() or 'stream' in filename:
                continue

            sources.append(filename)
        elif filename.endswith('.h'):
            headers.append(filename)

# Define flags based on platform.
if sys.platform == 'win32':
    flags = ['/Od', '/EHsc', '/MT']
    link_flags = []
else:
    flags = [
        '-c', '-O3',
        '-fPIC', '-Wall', '-Wno-parentheses',  # '-Werror=switch',
        '-D_FILE_OFFSET_BITS=64',
        '-D_LARGEFILE64_SOURCE',
        '-ggdb',
        '-fpack-struct=1',
    ]
    link_flags = ['-fPIC', '-lstdc++']

_irig106 = Extension(
    '_irig106',
    sources,
    depends=headers,
    extra_compile_args=flags,
    extra_link_args=link_flags,

    # This causes errors on unixy systems.
    # include_dirs=[os.path.join('libirig106', 'src')],
)

setup(
    name='libirig106',
    ext_modules=[_irig106],
    packages=['irig106'],
)
