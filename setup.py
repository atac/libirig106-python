
from setuptools import setup, Extension
import distutils.sysconfig
import os
import platform
import sys


# Find source and header files.
sources = []
headers = []
for pth, _, filenames in os.walk('libirig106/src'):
    for filename in filenames:
        filename = os.path.join(pth, filename)
        if filename.endswith(('.c')):
            if 'analog' in filename.lower() or 'stream' in filename:
                print filename
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
        '-fPIC', '-Wall', '-Wno-parentheses',  # '-Werror=switch',
        '-D_FILE_OFFSET_BITS=64',
        '-D_LARGEFILE64_SOURCE',
        '-ggdb',
        '-fpack-struct=1',
    ]

    platform.mac_ver()
    if platform.system() in ['Darwin', 'FreeBSD']:
        os.environ.setdefault('CC', 'clang')
        os.environ.setdefault('CXX', 'clang++')
        orig_customize_compiler = distutils.sysconfig.customize_compiler

        def customize_compiler(compiler):
            orig_customize_compiler(compiler)
            compiler.compiler[0] = os.environ['CC']
            compiler.compiler_so[0] = os.environ['CXX']
            compiler.compiler_cxx[0] = os.environ['CXX']
            compiler.linker_so[0] = os.environ['CXX']
            return compiler
        distutils.sysconfig.customize_compiler = customize_compiler
        flags.append('-stdlib=libc++')
        if platform.system() == 'Darwin':
            flags.append('-mmacosx-version-min=10.7',)
            # if tuple(map(int, platform.mac_ver()[0].split('.'))) >= (10, 9):
            #     flags.append(
            #         '-Wno-error=unused-command-line-argument-hard-error-in-future',  # noqa
            #     )

    flags = ['-c', '-O3'] + flags

    if platform.system() == 'FreeBSD':
        link_flags = ['-fPIC', '-lc++']
    else:
        link_flags = ['-fPIC', '-lstdc++']


c_extension = Extension(
    '_irig106',
    sources,
    # include_dirs=[os.path.join('.', 'src')],
    depends=headers,
    extra_compile_args=flags,
    extra_link_args=link_flags,
)

setup(
    name='libirig106',
    ext_modules=[c_extension],
    packages=['Py106'],
)
