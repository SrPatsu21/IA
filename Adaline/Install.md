# BUILD

```shell
sudo apt install gnuplot libboost-all-dev
wget https://github.com/dstahlke/gnuplot-iostream/blob/master/gnuplot-iostream.h
mkdir build
cd build
cmake ..
cmake --build .
./adaline
```

## Build for windows (not from)

```shell
sudo apt update
sudo apt install mingw-w64 wine

mkdir build-windows
cd build-windows

cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-mingw64.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .

wine ./adaline.exe
```
