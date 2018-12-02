import os
import sys
import pathlib
import platform
import setuptools
import setuptools.command.build_ext


class CMakeExtension(setuptools.Extension):
    def __init__(self, name):
        super(CMakeExtension, self).__init__(name, sources=[])


class build_ext(setuptools.command.build_ext.build_ext):
    def run(self):
        for ext in self.extensions:
            self.build_cmake(ext)
        super(build_ext, self).run()

    def build_cmake(self, ext):
        cwd = pathlib.Path().absolute()

        # These dirs will be created in build_py, so if you don't have
        # any python sources to bundle, the dirs will be missing
        build_temp = pathlib.Path(self.build_temp)
        build_temp.mkdir(parents=True, exist_ok=True)
        extdir = pathlib.Path(
            self.get_ext_fullpath(ext.name)).absolute().parent

        cfg = 'Debug' if self.debug else 'Release'

        cmake_args = [
            "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=" + str(extdir),
            "-DPYTHON_EXECUTABLE=" + sys.executable
        ]

        build_args = ['--config', cfg]

        if platform.system() != 'Windows':
            build_args += ['--', '-j%d' % os.cpu_count()]
            cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
        else:
            cmake_args += ['-DCMAKE_GENERATOR_PLATFORM=x64',
                           '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(
                               cfg.upper(), extdir)]
            build_args += ['--', '/m']

        if platform.system() == 'Darwin':
            os.environ['CXXFLAGS'] = "-stdlib=libc++"

        os.chdir(str(build_temp))
        if not os.path.exists("CMakeCache.txt"):
            self.spawn(['cmake', str(cwd)] + cmake_args)
        if not self.dry_run:
            self.spawn(
                ['cmake', '--build', '.', '--target', 'core'] + build_args)
        os.chdir(str(cwd))


def sources():
    result = []
    cwd = pathlib.Path().absolute()
    for dirname in ["src"]:
        for item in os.scandir(cwd.joinpath(dirname)):
            if item.is_file() and ".cpp" in str(item):
                result.append(item.path)
    return result


def main():
    setuptools.setup(
        name='pydap4',
        version='0.1.0',
        packages=setuptools.find_packages("src"),
        package_dir={'pydap4': os.path.join(
            'src', 'pydap4')},
        package_data={'pydap4': []},
        ext_modules=[
            CMakeExtension(name="pydap4.core")
        ],
        cmdclass={
            'build_ext': build_ext,
        },
        test_suite='nose.collector',
        tests_require=['nose'],
    )


if __name__ == "__main__":
    main()
