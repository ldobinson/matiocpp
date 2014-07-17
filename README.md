#  matiocpp
matiocpp is a c++ library to enable easy loading/writing matlab files using C++. matiocpp is licensed under GPLv3.

## Prerequisites
Currently, matiocpp has only been built and tested with Linux 64bit (distributions: Debian 6.0 and Redhat 6.x) and Windows 32bit ([mingw32](www.mingw32.org) and [MXE](www.mxe.cc)).

Base requirements:<br>
1. [Boost](www.boost.org) (tested with >1.49), in particular boost::filesystem must be v3!<br>
1. [matio](http://sourceforge.net/projects/matio/) tested with 1.5.x and above

## Compiling and installing
Compiling and installing matiocpp is done by using cmake (2.8.x required):

    git clone https://github.com/cipherz/matiocpp.git
    cd matiocpp
    mkdir build
    cd build
    cmake ..
    make
    cpack .

Depending on your system (windows: tgz or linux: deb/tgz) can now be installed. The following cmake parameters are supported:

    -DCMAKE_BUILD_TYPE - Can be [Debug,Release]
    -DWITH_STATIC - Compiles static library [1,0]
    -DWITH_SHARED - Compiles shared library [1,0]

Cross-compiling matiocpp with MXE has been tested and should work if you built matio for MXE and then built with -DCMAKE_TOOLCHAIN_FILE=mxetoolchain.
