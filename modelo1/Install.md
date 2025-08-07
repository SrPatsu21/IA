# Installation tutorial

## Install

```shell
mkdir -p libs
cd libs
git clone https://gitlab.com/libeigen/eigen.git
git clone https://github.com/opencv/opencv.git
cd ..
```

## Install for windows

```shell
sudo apt install mingw-w64
```

## Build

```shell
mkdir -p build/linux && cd build/linux
cmake ../.. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF
make -j$(nproc)
./ia_basica
cd ../..
```

## Build Windows

```shell
mkdir -p build/windows && cd build/windows
cmake ../.. -DCMAKE_TOOLCHAIN_FILE=../../mingw-toolchain.cmake -DCMAKE_BUILD_TYPE=Release
make
wine ./ia_basica.exe
cd ../..
```
