# BUILD

```shell
mkdir build
cd build
cmake ..
cmake --build .
./water_bucket_tree.out
```

## Build for windows (not from)

```shell
sudo apt update
sudo apt install mingw-w64 wine

mkdir build-windows
cd build-windows

cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-mingw64.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .

wine ./water_bucket_tree.exe
```
